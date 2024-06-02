#include <lcom/lcf.h>

#include "graphic.h"

#include "VBE.h"

vbe_mode_info_t mode_info;

static uint8_t *video_mem;       /* Process (virtual) address to which VRAM is mapped */
static void *buffer;             /* Back buffer for double buffering */
static void *current;            /* Current buffer for double buffering */

static unsigned h_res;           /* Horizontal resolution in pixels */
static unsigned v_res;           /* Vertical resolution in pixels */
static unsigned bits_per_pixel;  /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel; /* Number of VRAM bytes per pixel*/
static unsigned vram_size;       /* VRAM's size */

void *(vg_init) (uint16_t mode) {
  /* 1. Initialize static global variables */
  if (vbe_get_mode_info(mode, &mode_info)) {
    printf("%s: vbe_get_mode_info(mode: 0x%x, mode_info) error\n", __func__, mode);
    return NULL;
  }
  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bits_per_pixel = mode_info.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;

  /* 2. Map VRAM to the process' address space */
  struct minix_mem_range mr;
  unsigned int vram_base = mode_info.PhysBasePtr;  /* VRAM's physical address */
  vram_size = h_res * v_res * bytes_per_pixel; /* VRAM's size */

  struct minix_mem_range mr_buffer;
  unsigned int buffer_base = vram_base + vram_size;

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr_buffer.mr_base = (phys_bytes) buffer_base;

  mr.mr_limit = mr.mr_base + vram_size * 2;
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) {
    printf("%s: sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) error\n", __func__);
    return NULL;
  }

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED) {
    printf("%s: vm_map_phys(SELF, mr.mr_base, vram_size: %d) error\n", __func__, vram_size);
    return NULL;
  }

  buffer = vm_map_phys(SELF, (void *) mr_buffer.mr_base, vram_size);
  if (buffer == MAP_FAILED) {
    printf("%s: vm_map_phys(SELF, mr_buffer.mr_base, vram_size: %d) error\n", __func__, vram_size);
    return NULL;
  }

  memset(video_mem, 0, vram_size);
  memset(buffer, 0, vram_size);

  /* 3. Set the desired graphics mode using a linear frame buffer */
  reg86_t r;

  /* Specifiy the appropriate register values */
  memset(&r, 0, sizeof(r)); /* zero the structure */
  r.intno = 0x10;
  r.ah = 0x4F;
  r.al = 0x02;
  r.bx = mode | BIT(14);
  /* Make the BIOS call */
  if (sys_int86(&r)) {
    printf("%s: sys_int86(&r) error\n", __func__);
    return NULL;
  }

  current = buffer;

  return video_mem;
}

int(video_swap_buffers)() {
  /* 1. Get display start */
  reg86_t r;

  /* Specifiy the appropriate register values */
  memset(&r, 0, sizeof(r)); /* zero the structure */
  r.intno = 0x10;      /* BIOS video services */
  r.ah = 0x4F;
  r.al = 0x07;
  r.bh = 0x00;
  r.bl = 0x01;

  /* Make the BIOS call */
  if (sys_int86(&r)) {
    printf("%s: sys_int86(&r) error\n", __func__);
    return 1;
  }

  // DX = First Displayed Scan Line
  uint16_t dx;
  if (r.dx == 0) {
    dx = v_res;
    current = video_mem;
    memcpy(current, buffer, vram_size);
  } else if (r.dx == v_res) {
    dx = 0;
    current = buffer;
    memcpy(current, video_mem, vram_size);
  }
  else {
    printf("%s: DX (%d) error\n", __func__, r.dx);
    return 1;
  }

  /* 2. Set display start */

  /* Specifiy the appropriate register values */
  memset(&r, 0, sizeof(r));
  r.intno = 0x10;
  r.ah = 0x4F;
  r.al = 0x07;
  r.bh = 0x00;
  r.bl = 0x00;
  r.cx = 0;  // CX = First Displayed Pixel In Scan Line
  r.dx = dx; // DX = First Displayed Scan Line

  /* Make the BIOS call */
  if (sys_int86(&r)) {
    printf("%s: sys_int86(&r) error\n", __func__);
    return 1;
  }

  return 0;
}

rgb_8_8_8_t(video_get_colors)(uint32_t color) {
  uint8_t r = color >> mode_info.RedFieldPosition % BIT(mode_info.RedMaskSize);
  uint8_t g = color >> mode_info.GreenFieldPosition % BIT(mode_info.GreenMaskSize);
  uint8_t b = color >> mode_info.BlueFieldPosition % BIT(mode_info.BlueMaskSize);
  rgb_8_8_8_t colors = {r, g, b};
  return colors;
}

uint32_t(video_get_color)(rgb_8_8_8_t colors) {
  uint32_t color = (colors.red << mode_info.RedFieldPosition) | (colors.green << mode_info.GreenFieldPosition) | (colors.blue << mode_info.BlueFieldPosition);
  return color;
}

int(video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x < 0 || x >= h_res || y < 0 || y >= v_res) {
    printf("%s: pixel (%d, %d) error\n", __func__, x, y);
    return 1;
  }

  uint8_t *byte = current;
  byte += (x + y * h_res) * bytes_per_pixel;

  if (!memcpy(byte, &color, bytes_per_pixel)) {
    printf("%s: memcpy(byte: 0x%x, color: 0x%x, bytes_per_pixel: %d)\n", __func__, *byte, color, bytes_per_pixel);
    return 1;
  }

  return 0;
}

int(video_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) {
    if (video_draw_pixel(x + i, y, color)) {
      printf("%s: video_draw_pixel(x + i: %d, y: %d, color: %d) error\n", __func__, x + i, y, color);
      return 1;
    }
  }
  return 0;
}

int(video_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height; i++) {
    if (video_draw_hline(x, y + i, width, color)) {
      printf("%s: video_draw_hline(x: %d, y + i: %d, width: %d, color: %d) error\n", __func__, x, y + i, width, color);
      return 1;
    }
  }
  return 0;
}

int(video_draw_pixmap)(uint16_t x, uint16_t y, xpm_image_t *image) {
  for (uint16_t row = 0; row < image->height; row++)
    for (uint16_t col = 0; col < image->width; col++)
      if (video_draw_pixel(x + col, y + row, *(image->bytes + col + row * image->width))) {
        printf("%s: video_draw_pixel(x + col: %d, y + row: %d, *(image->bytes + col + row * image->width): 0x%x) error\n", __func__, x + col, y + row, *(image->bytes + col + row * image->width));
        return 1;
      }
  return 0;
}

int(video_clean)(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf) {
  for (uint16_t y = yi; y < yf; y++)
    for (uint16_t x = xi; x < xf; x++)
      if (video_draw_pixel(x, y, 0)) {
        printf("%s: video_draw_pixel(x: %d, y: %d, 0) error\n", __func__, x, y);
        return 1;
      }
  return 0;
}

int (video_draw_background)(uint8_t *background) {
  memcpy(current, background, vram_size);
  return 0;
}
