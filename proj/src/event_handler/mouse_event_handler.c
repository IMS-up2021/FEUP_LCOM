#include "mouse_event_handler.h"

void handle_mouse_event(struct packet pp) {
    switch (state) {
        case MAIN_MENU:
            handle_mouse_menu_event(pp);
            break;
        case PLAYING:
            handle_mouse_game_event(pp);
            break;
        case LEADERBOARD:
            handle_mouse_leaderboard_event(pp);
            break;
        case INSTRUCTIONS:
            handle_mouse_difficulty_event(pp);
            break;
        case EXIT:
            break;
    }
}

void handle_mouse_game_event(struct packet pp) {
    if (pp.lb || pp.rb) {
        //TODO: something
    }
}

void handle_mouse_menu_event(struct packet pp) {
    update_mouse(pp.delta_x, pp.delta_y);
    if (pp.lb) {
        check_click();
    }
}

void handle_mouse_leaderboard_event(struct packet pp) {
    update_mouse(pp.delta_x, pp.delta_y);
}

void handle_mouse_instructions_event(struct packet pp) {
    update_mouse(pp.delta_x, pp.delta_y);
    if (pp.lb) {
        check_click();
    }
}
