#include <stdio.h>
#include "drivers/mouse.h"
#include "game/game_state.h"

void handle_mouse_event(struct packet pp);
void handle_mouse_game_event(struct packet pp);
void handle_mouse_menu_event(struct packet pp);
void handle_mouse_leaderboard_event(struct packet pp);
void handle_mouse_instructions_event(struct packet pp);
