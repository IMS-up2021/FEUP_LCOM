#include "graphic.h"
#include <lcom/lcf.h>
#include <math.h>

void *vg_init(uint16_t mode) {
    if (set_frame_buffer(mode) != 0) {
        printf("Error setting frame buffer\n");
        return NULL;
    }
    if (set_graphic_mode(mode) != 0) {
        printf("Error setting graphic mode\n");
        return NULL;
    }
    return frame_buffer;
}

int set_graphic_mode(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = 0x10;
    reg86.ah = 0x4F;
    reg86.al = 0x02;
    reg86.bx = submode | BIT(14);

    if (sys_int86(&reg86) != 0) {
        printf("Set graphic mode failed\n");
        return 1;
    }
    return 0;
}

int set_text_mode() {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = 0x10;
    reg86.ah = 0x00;
    reg86.al = 0x03;

    if (sys_int86(&reg86) != 0) {
        printf("Set text mode failed\n");
        return 1;
    }
    return 0;
}

int set_frame_buffer(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info));

    if (vbe_get_mode_info(mode, &mode_info)) {
        return 1;
    }

    unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

    struct minix_mem_range physic_addresses;
    physic_addresses.mr_base = mode_info.PhysBasePtr;
    physic_addresses.mr_limit = physic_addresses.mr_base + frame_size;

    center_x = mode_info.XResolution / 2;
    center_y = mode_info.YResolution / 2;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) {
        printf("Physical memory allocation error\n");
        return 1;
    }

    frame_buffer = vm_map_phys(SELF, (void *) physic_addresses.mr_base, frame_size);
    second_buffer = malloc(frame_size);

    if (frame_buffer == NULL) {
        printf("Virtual memory allocation error\n");
        return 1;
    }

    return 0;
}

int print_xpm(xpm_image_t img, uint16_t x, uint16_t y) {
    if (y + img.height > mode_info.YResolution) {
        y = mode_info.YResolution - img.height;
    }
    if (x + img.width > mode_info.XResolution) {
        x = mode_info.XResolution - img.width;
    }

    uint64_t count = 0;
    uint16_t initial_x = x;

    for (int h = 0; h < img.height; h++) {
        for (int w = 0; w < img.width; w++) {
            uint64_t offset = ((y * mode_info.XResolution) + x) * (mode_info.BytesPerScanLine / mode_info.XResolution);
            void *address = (void *) ((char *) second_buffer + offset);

            uint8_t bytes_per_pixel = img.size / (img.height * img.width);
            uint32_t color = 0;

            for (size_t i = 0; i < 3; i++) {
                color |= *(img.bytes + (count * bytes_per_pixel) + i) << (i * 8);
            }

            if (color == 0x00b140) {  // Transparent color
                count++;
                x++;
                continue;
            }

            memcpy(address, img.bytes + (count * bytes_per_pixel), mode_info.BytesPerScanLine / mode_info.XResolution);
            x++;
            count++;
        }
        y++;
        x = initial_x;
    }
    return 0;
}

void copy_buffer() {
    memcpy(frame_buffer, second_buffer, mode_info.XResolution * mode_info.YResolution * (mode_info.BytesPerScanLine / mode_info.XResolution));
}

void draw_board(xpm_image_t board) {
    uint8_t block_side = (mode_info.YResolution - 30) / 16;
    uint8_t x = center_x - (block_side * 5) - 10;
    print_xpm(board, x, 10);
}

void draw_board_block(xpm_image_t img, uint8_t x, uint8_t y) {
    print_xpm(img, (img.width * x) + (center_x - 5 * img.width), ((mode_info.YResolution - 55) - (y * img.height)));
}

void draw_char(xpm_image_t font, uint16_t x, uint16_t y, uint8_t scale, uint8_t *pos) {
    uint16_t initial_x = x;

    for (int i = 0; i < 7; i++) {
        for (int k = 0; k < scale; k++) {
            for (int j = 0; j < 6; j++) {
                for (int l = 0; l < scale; l++) {
                    uint64_t offset = ((y * mode_info.XResolution) + x) * (mode_info.BytesPerScanLine / mode_info.XResolution);
                    void *addr = (void *) ((char *) second_buffer + offset);

                    uint8_t bytes_per_pixel = font.size / (font.height * font.width);
                    uint32_t color = 0;

                    for (size_t off = 0; off < 3; off++) {
                        color |= *(pos + (j + i * font.width) * bytes_per_pixel + off) << (off * 8);
                    }

                    if (color == 0x00b140) {  // Transparent color
                        x++;
                        continue;
                    }

                    memcpy(addr, pos + ((i * font.width) + j) * bytes_per_pixel, mode_info.BytesPerScanLine / mode_info.XResolution);
                    x++;
                }
            }
            y++;
            x = initial_x;
        }
    }
}

int draw_score_background(xpm_image_t square) {
    uint16_t x = mode_info.XResolution - square.width - 50;
    uint16_t y = 10;
    print_xpm(square, x, y);
    return x;
}

int normalize_color(uint32_t color, uint32_t *new_color) {
    if (mode_info.BitsPerPixel == 32) {
        *new_color = color;
    } else {
        *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
    }
    return 0;
}

uint32_t direct_mode(uint32_t r, uint32_t g, uint32_t b) {
    return (r << mode_info.RedFieldPosition) | (g << mode_info.GreenFieldPosition) | (b << mode_info.BlueFieldPosition);
}

uint32_t indexed_mode(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
    return (first + (row * n + col) * step) % (1 << mode_info.BitsPerPixel);
}

uint32_t Red(unsigned j, uint8_t step, uint32_t first) {
    return (r(first) + j * step) % (1 << mode_info.RedMaskSize);
}

uint32_t Green(unsigned i, uint8_t step, uint32_t first) {
    return (g(first) + i * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t Blue(unsigned j, unsigned i, uint8_t step, uint32_t first) {
    return (b(first) + (i + j) * step) % (1 << mode_info.BlueMaskSize);
}

uint32_t r(uint32_t first) {
    return ((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition);
}

uint32_t g(uint32_t first) {
    return ((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition);
}

uint32_t b(uint32_t first) {
    return ((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition);
}
