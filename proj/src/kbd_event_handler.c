#include "kbd_event_handler.h"

void handle_kbd_event(uint8_t scan_code[2]) {
    switch (state) {
        case MAIN_MENU:
            break;
        case PLAYING:
            handle_kbd_game_event(scan_code);
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
    bool two_byte = (scan_code[0] == TWO_BYTE_SC);
    uint8_t key_code = two_byte ? scan_code[1] : scan_code[0];

    if (two_byte) {
        switch (key_code) {
                move_up();
                break;
            case DOWN_ARROW:
                move_down();
                break;
            default: break;
        }
    } // else if ()
}

