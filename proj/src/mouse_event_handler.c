#include "mouse_event_handler.h"
#include "draw.h"
#include "game_state.h"

Button *play_button;
Position mouse_pos;

void handle_mouse_event(struct packet pp) {
    switch (state) {
        case MAIN_MENU:
            handle_mouse_menu_event(pp);
            break;
        case PLAYING:
            handle_mouse_game_event(pp);
            break;
        case INSTRUCTIONS:
            handle_mouse_instructions_event(pp);
            break;
        case EXIT:
            break;
    }
}

void handle_mouse_game_event(struct packet pp) {
    update_mouse(pp.delta_x, pp.delta_y);
}

void handle_mouse_menu_event(struct packet pp) {
    update_mouse(pp.delta_x, pp.delta_y);
    if (!pp.lb) return;
    if (mouse_pos.x >= play_button->pos_x && mouse_pos.x <= play_button->pos_x + play_button->width &&
        mouse_pos.y >= play_button->pos_y && mouse_pos.y <= play_button->pos_y + play_button->height)
        play_button->on_click();
    else if (mouse_pos.x >= quit->pos_x && mouse_pos.x <= quit->pos_x + quit->width &&
             mouse_pos.y >= quit->pos_y && mouse_pos.y <= quit->pos_y + quit->height)
        quit->on_click();
    
}

void handle_mouse_instructions_event(struct packet pp) {
    update_mouse(pp.delta_x, pp.delta_y);
}
