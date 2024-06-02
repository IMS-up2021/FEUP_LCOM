#include "timer_event_handler.h"


void handle_timer_event(int game_state) {
  switch (state) {
    case MAIN_MENU:
      handle_menu();
      break;
      
    case PLAYING:
      handle_game();
      break;
      
    case LEADERBOARD:
      handle_leaderboard();
      break;

    case INSTRUCTIONS:
      handle_instructions();
      
    case EXIT:
      break;
  }
}

void handle_menu() {
  //erase_mouse();
  //draw_title_screen();
  //draw_mouse();
}

void handle_game() {
  //draw_board();
  //draw_current_score();
}

void handle_leaderboard() {
  //erase_mouse();
  //leaderboard();
  //draw_mouse();
}

void handle_instructions() {
  //erase_mouse();
  //draw_instructions();
  //draw_mouse();
}
