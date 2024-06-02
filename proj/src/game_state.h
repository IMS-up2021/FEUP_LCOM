#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "utils.h"
#include "entity.h"

typedef enum {
  PLAYING,
  MAIN_MENU,
  INSTRUCTIONS,
  LEADERBOARD,
  EXIT
} game_state;

typedef struct Position {
  uint16_t x;
  uint16_t y;
} Position;

extern game_state state;

void play(void);
void leaderboard(void);
void instructions(void);
void update_mouse(int16_t x, int16_t y);

void (move_up)();
void (move_down)();


#endif
