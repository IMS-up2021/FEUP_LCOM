#include "utils.h"

bool is_char (char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

char get_key(uint8_t keycode) {
    if (keycode < Q_T || keycode > M_T) return "";
    if (keycode > P_T) keycode -= 4;
    if (keycode > L_T) keycode -= 5;
    return "QWERTYUIOPASDFGHJKLZXCVBNM"[keycode - Q_T];
}