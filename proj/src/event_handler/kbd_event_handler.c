#include "kbd_event_handler.h"

void handle_kbd_event(uint8_t scan_code[2]) {
    switch (state) {
        case MAIN_MENU:
            break;
        case PLAYING:
            handle_kbd_game_event(scan_code);
            break;
        case LEADERBOARD:
            handle_kbd_leaderboard_event(scan_code);
            break;
        case INSTRUCTIONS:
            handle_kbd_instructions_event(scan_code);
            break;
        case EXIT:
            break;
    }
}

void handle_kbd_instructions_event(uint8_t scan_code[2]) {
    if (scan_code[0] == ESC_BREAK) {
        state = MAIN_MENU;
    }
}

void handle_kbd_game_event(uint8_t scan_code[2]) {
    if (scan_code[0] == TWO_BYTE_SC) {
        switch (scan_code[1]) {
            case LEFT_ARROW:
                move_left();
                break;
            case RIGHT_ARROW:
                move_right();
                break;
            case DOWN_ARROW:
                piece_fall();
                break;
            default:
                break;
        }
    } else if (scan_code[0] == SPACE) {
        drop_piece();
    }
}


void handle_kbd_leaderboard_event(uint8_t scan_code[2]) {
    if (scan_code[0] == ESC_BREAK) {
        state = MAIN_MENU;
    }
}
