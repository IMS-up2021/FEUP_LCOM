#include "game_state.h"
#include "draw.h"

game_state state = MAIN_MENU;
bool game_over = false;
Position mouse_pos = {300, 320};

void update_mouse(int16_t x, int16_t y) {
    int32_t new_x = mouse_pos.x + x;
    int32_t new_y = mouse_pos.y - y;

    mouse_pos.x = (new_x >= 0 && new_x < x_max) ? new_x : mouse_pos.x;
    mouse_pos.y = (new_y >= 0 && new_y < y_max) ? new_y : mouse_pos.y;
}


void (move_up)(Player *player) {
     if (player->y_pos != 0){
        player->y_pos--;     
    }
}

void move_down(Player *player) {
    if (player->y_pos - player->height <= 600){
        player->y_pos++;     
    }
}

