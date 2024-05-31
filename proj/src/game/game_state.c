#include "game_state.h"

game_state state = MAIN_MENU;
bool game_over = false;
Position mouse_pos = {300, 320};

void update_mouse(int16_t x, int16_t y) {
    int32_t new_x = mouse_pos.x + x;
    int32_t new_y = mouse_pos.y - y;

    mouse_pos.x = (new_x >= 0 && new_x < 800) ? new_x : mouse_pos.x;
    mouse_pos.y = (new_y >= 0 && new_y < 600) ? new_y : mouse_pos.y;
}