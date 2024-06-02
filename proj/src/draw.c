#include <lcom/lcf.h>
#include <math.h>

#include "draw.h"
#include "game_state.h"
#include "ui.h"
#include "graphic.h"


#include "xpm/0.h"
#include "xpm/1.h"
#include "xpm/2.h"
#include "xpm/3.h"
#include "xpm/4.h"
#include "xpm/5.h"
#include "xpm/6.h"
#include "xpm/7.h"
#include "xpm/8.h"
#include "xpm/9.h"

vbe_mode_info_t mode_info;

Button *play_button;
Cursor *cursor_menu;
Button *quit;
Ball *ball;
Wall *wall;

Digit digits[10];

uint32_t score = 0;

uint16_t x_max = 1024;
uint16_t y_max = 768;

game_state state;


#define WIDTH_NUMBER 20
#define COLOR_BACKGROUND 0x000000
#define WALL_COLOR 0xFFFFFF

bool init_digits(void) {
  xpm_image_t image;
  
  uint8_t *sprite = xpm_load((xpm_map_t)xpm_0, XPM_INDEXED, &image);
  digits[0] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_1, XPM_INDEXED, &image);
  digits[1] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_2, XPM_INDEXED, &image);
  digits[2] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_3, XPM_INDEXED, &image);
  digits[3] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_4, XPM_INDEXED, &image);
  digits[4] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_5, XPM_INDEXED, &image);
  digits[5] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_6, XPM_INDEXED, &image);
  digits[6] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_7, XPM_INDEXED, &image);
  digits[7] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_8, XPM_INDEXED, &image);
  digits[8] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_9, XPM_INDEXED, &image);
  digits[9] = (Digit){ image.width, image.height, sprite };
  return true;
}

int draw_ball(Ball *ball) {
  if (!ball) {
    printf("%s: NULL ball\n", __func__);
    return 1;
  }

  // if (draw_sprite(ball->x_possprite, ball->x_pos, ball->y_pos)) {
  //   printf("%s: draw_sprite(ball->x_possprite, ball->x_pos: %d, ball->y_pos: %d) error\n", __func__, ball->x_pos, ball->y_pos);
  //   return 1;
  // }

  return 0;
}

int draw_button(Button *button) {
  if (!button) {
    printf("%s: NULL button\n", __func__);
    return 1;
  }

  // if (!button->selected && draw_sprite(button->sprite, button->pos_x, button->pos_y)) {
  //   printf("%s: draw_sprite(button->sprite, button->pos_x: %d, button->pos_y: %d) error\n", __func__, button->pos_x, button->pos_y);
  //   return 1;
  // }
  // else if (button->selected && draw_sprite(button->sprite_selected, button->pos_x, button->pos_y)) {
  //   printf("%s: draw_sprite(button->sprite_selected, button->pos_x: %d, button->pos_y: %d) error\n", __func__, button->pos_x, button->pos_y);
  //   return 1;
  // }

  return 0;
}

int draw_cursor(Cursor *cursor) {
  if (!cursor) {
    printf("%s: NULL cursor\n", __func__);
    return 1;
  }

  // if (draw_sprite(cursor->sprite, cursor->pos_x, cursor->pos_y)) {
  //   printf("%s: draw_sprite(cursor->sprite, cursor->pos_x: %d, cursor->pos_y: %d) error\n", __func__, cursor->pos_x, cursor->pos_y);
  //   return 1;
  // }

  return 0;
}

int draw_game() {
  // if (draw_times(x_max * 0.05, y_max * 0.05)) {
  //   printf("%s: draw_times(x_max * 0.05: %f, y_max * 0.05: %f)\n", __func__, x_max * 0.05, y_max * 0.05);
  //   return 1;
  // }

  if (draw_number(score, x_max * 0.95, y_max * 0.05)) {
    printf("%s: draw_number(score: %d, x_max * 0.95: %f, y_max * 0.05: %f) error\n", __func__, score, x_max * 0.95, y_max * 0.05);
    return 1;
  }

  if (draw_ball(ball)) {
    printf("%s: draw_ball(ball) error\n", __func__);
    return 1;
  }

  if (draw_wall(wall)) {
    printf("%s: draw_wall(wall) error\n", __func__);
    return 1;
  }

  return 0;
}

int draw_menu() {
  if (draw_button(play_button)) {
    printf("%s: draw_button(play) error\n", __func__);
    return 1;
  }

  if (draw_button(quit)) {
    printf("%s: draw_button(quit) error\n", __func__);
    return 1;
  }

  if (draw_cursor(cursor_menu)) {
    printf("%s: draw_cursor(cursor_menu) error\n", __func__);
    return 1;
  }

  return 0;
}



bool draw_digit(Digit *digit, uint16_t x, uint16_t y) {
  for (uint16_t h = 0; h < digit->height; h++) {
    for (uint16_t w = 0; w < digit->width; w++) {
      if (video_draw_pixel(x + w, y + h, digit->sprite[w + h * digit->width])) {
        printf("%s: video_draw_pixel(x + w: %d, y + h: %d, color: 0x%x) error\n", __func__, x + w, y + h, digit->sprite[w + h * digit->width]);
        return 1;
      }
    }
  }

  return 0;
}

int draw_number(uint32_t number, uint16_t x, uint16_t y) {

  uint32_t reverse = 0;

  for (; number > 0; number /= 10) reverse = reverse * 10 + (number % 10);

  printf("%d", reverse);

  for (uint32_t digit = reverse; digit > 0; digit /= 10) {
    draw_digit(&digits[digit % 10], x, y);
    x += digits[digit % 10].width;
  }

  return 0;
}


int draw_frame() {
  if (video_draw_background(COLOR_BACKGROUND)) {
    printf("%s: video_draw_background(COLOR_BACKGROUND: 0x%x) error\n", __func__, COLOR_BACKGROUND);
    return 1;
  }

  switch (state)
  {
  case (MAIN_MENU):
    return draw_menu();
  case (PLAYING):
    return draw_game();
  
  default:
    break;
  }

  return 0;
}

int draw_wall(Wall *wall) {
  if (!wall) {
    printf("%s: wall is NULL\n", __func__);
    return 1;
  }

  if (video_draw_rectangle(wall->x_pos, wall->y_pos, wall->width, wall->height, wall->top)) {
    printf("%s: video_draw_rectangle(wall->x_pos; %d, wall->y_pos: %d, wall->width: %d, wall->height: %d, WALL_COLOR: 0x%x) error\n", __func__, wall->x_pos, wall->y_pos, wall->width, wall->height, WALL_COLOR);
    return 1;
  }

  return 0;
}

