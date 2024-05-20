#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include <lcom/xpm.h>

extern vbe_mode_info_t mode_info;
extern void *frame_buffer;
extern void *second_buffer;
extern uint16_t center_x;
extern uint16_t center_y;

void *vg_init(uint16_t mode);
int set_graphic_mode(uint16_t submode);
int set_text_mode();
int set_frame_buffer(uint16_t mode);
int print_xpm(xpm_image_t img, uint16_t x, uint16_t y);
void copy_buffer();
void draw_board(xpm_image_t board);
void draw_board_block(xpm_image_t img, uint8_t x, uint8_t y);
void draw_char(xpm_image_t font, uint16_t x, uint16_t y, uint8_t scale, uint8_t *pos);
int draw_score_background(xpm_image_t square);
int normalize_color(uint32_t color, uint32_t *new_color);
uint32_t direct_mode(uint32_t r, uint32_t g, uint32_t b);
uint32_t indexed_mode(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);
uint32_t Red(unsigned j, uint8_t step, uint32_t first);
uint32_t Green(unsigned i, uint8_t step, uint32_t first);
uint32_t Blue(unsigned j, unsigned i, uint8_t step, uint32_t first);
uint32_t r(uint32_t first);
uint32_t g(uint32_t first);
uint32_t b(uint32_t first);

#endif // GRAPHICS_H
