#include "draw.h"
#include "game_state.h"
#include <lcom/lcf.h>

extern Ball *ball;
extern Wall *wall;
extern uint32_t score;

extern Button *play;
extern Button *quit;
extern Cursor *cursor_menu;

extern Sprite *zero;
extern Sprite *one;
extern Sprite *two;
extern Sprite *three;
extern Sprite *four;
extern Sprite *five;
extern Sprite *six;
extern Sprite *seven;
extern Sprite *eight;
extern Sprite *nine;

game_state state;

extern uint16_t x_max;
extern uint16_t y_max;

#define WIDTH_NUMBER 20
#define COLOR_BACKGROUND 0x000000
#define WALL_COLOR 0xFFFFFF

int draw_ball(Ball *ball) {
  if (!ball) {
    printf("%s: NULL ball\n", __func__);
    return 1;
  }

  if (draw_sprite(ball->sprite, ball->x, ball->y)) {
    printf("%s: draw_sprite(ball->sprite, ball->x: %d, ball->y: %d) error\n", __func__, ball->x, ball->y);
    return 1;
  }

  return 0;
}

int draw_button(Button *button) {
  if (!button) {
    printf("%s: NULL button\n", __func__);
    return 1;
  }

  if (!button->selected && draw_sprite(button->sprite, button->x, button->y)) {
    printf("%s: draw_sprite(button->sprite, button->x: %d, button->y: %d) error\n", __func__, button->x, button->y);
    return 1;
  }
  else if (button->selected && draw_sprite(button->sprite_selected, button->x, button->y)) {
    printf("%s: draw_sprite(button->sprite_selected, button->x: %d, button->y: %d) error\n", __func__, button->x, button->y);
    return 1;
  }

  return 0;
}

int draw_cursor(Cursor *cursor) {
  if (!cursor) {
    printf("%s: NULL cursor\n", __func__);
    return 1;
  }

  if (draw_sprite(cursor->sprite, cursor->x, cursor->y)) {
    printf("%s: draw_sprite(cursor->sprite, cursor->x: %d, cursor->y: %d) error\n", __func__, cursor->x, cursor->y);
    return 1;
  }

  return 0;
}

int draw_game() {
  if (draw_times(x_max * 0.05, y_max * 0.05)) {
    printf("%s: draw_times(x_max * 0.05: %f, y_max * 0.05: %f)\n", __func__, x_max * 0.05, y_max * 0.05);
    return 1;
  }

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
  if (draw_button(play)) {
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

int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y) {
  uint16_t height = sprite->height;
  uint16_t width = sprite->width;
  uint32_t background = sprite->background;
  uint32_t color;
  for (uint16_t h = 0; h < height; h++)
    for (uint16_t w = 0; w < width; w++) {
      color = sprite->colors[w + h * width];
      if (color == background)
        continue;
      if (video_draw_pixel(x + w, y + h, color)) {
        printf("%s: video_draw_pixel(x + w: %d, y + h: %d, color: 0x%x) error\n", __func__, x + w, y + h, color);
        return 1;
      }
    }
  return 0;
}

int draw_digit(uint8_t digit, uint16_t x, uint16_t y) {
  switch (digit) {
    case 0:
      if (draw_sprite(zero, x, y)) {
        printf("%s: draw_sprite(zero, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 1:
      if (draw_sprite(one, x, y)) {
        printf("%s: draw_sprite(one, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 2:
      if (draw_sprite(two, x, y)) {
        printf("%s: draw_sprite(two, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 3:
      if (draw_sprite(three, x, y)) {
        printf("%s: draw_sprite(three, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 4:
      if (draw_sprite(four, x, y)) {
        printf("%s: draw_sprite(four, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 5:
      if (draw_sprite(five, x, y)) {
        printf("%s: draw_sprite(five, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 6:
      if (draw_sprite(six, x, y)) {
        printf("%s: draw_sprite(six, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 7:
      if (draw_sprite(seven, x, y)) {
        printf("%s: draw_sprite(seven, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 8:
      if (draw_sprite(eight, x, y)) {
        printf("%s: draw_sprite(eight, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 9:
      if (draw_sprite(nine, x, y)) {
        printf("%s: draw_sprite(nine, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
  }
  return 0;
}

int draw_number(uint32_t number, uint16_t x, uint16_t y) {
  uint8_t digit;
  while (number > 0) {
    digit = number % 10;
    if (draw_digit(digit, x, y)) {
      printf("%s: draw_digit(digit: %d, x: %d, y: %d)\n", __func__, digit, x, y);
      return 1;
    }
    number /= 10;
    x -= WIDTH_NUMBER;
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
    printf("%s: video_draw_rectangle(wall->x; %d, wall->y: %d, wall->w: %d, wall->h: %d, WALL_COLOR: 0x%x) error\n", __func__, wall->x, wall->y, wall->w, wall->h, WALL_COLOR);
    return 1;
  }

  return 0;
}

