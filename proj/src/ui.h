#ifndef PROJ_UI_H
#define PROJ_UI_H

#include <lcom/lcf.h>
#include <stdbool.h>

typedef struct {
    const unsigned int pos_x;
    const unsigned int pos_y;
    const unsigned int width;
    const unsigned int height;
    const uint8_t *sprite;
    void (*on_click)(void);
} Button;


typedef struct {
    unsigned int pos_x;
    unsigned int pos_y;
    const unsigned int width;
    const unsigned int height;
    const uint8_t *sprite;
} Cursor;

typedef struct {
    const unsigned int width;
    const unsigned int height;
    const uint8_t *sprite;
} Digit;

Button *create_button(unsigned int pos_x, unsigned int pos_y, xpm_map_t xpm, void (*on_click)(void));
Cursor *create_cursor(unsigned int pos_x, unsigned int pos_y, xpm_map_t xpm);

#endif
