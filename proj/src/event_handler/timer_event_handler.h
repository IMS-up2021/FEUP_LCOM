#include <stdio.h>
#include "drivers/timer.h"
#include <lcom/timer.h>
#include "drivers/interrupt_manager.h"
#include "game/game_state.h"


void handle_timer_event();
void handle_menu();
void handle_game();
void handle_leaderboard();
void handle_instructions();
