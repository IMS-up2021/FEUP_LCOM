#include <stdio.h>
#include "drivers/kbd.h"
#include "game/game_state.h"

void handle_kbd_event(uint8_t scan_code[2]);
void handle_kbd_game_event(uint8_t scan_code[2]);
void handle_kbd_leaderboard_event(uint8_t scan_code[2]);
void handle_kbd_instructions_event(uint8_t scan_code[2]);

