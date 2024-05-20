#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H

#include <lcom/lcf.h>
#include "timer.h"
#include "kbd.h"
#include "mouse.h"
#include "graphic.h"

extern uint32_t timer_irq_set;
extern uint32_t kbd_irq_set;
extern uint32_t mouse_irq_set;
extern enum state_t { MENU, PLAYING, PAUSE, EXIT } state;
extern void *frame_buffer;
extern void *second_buffer;
extern uint16_t center_x;
extern uint16_t center_y;

int init();
int main_loop();
int leave();
void load_xpms();
void get_keys();
void get_scores();
void handle_timer_event();
void handle_kbd_event(uint8_t scancode[]);

/**
 * @brief Handles mouse events.
 * 
 * @param pp The packet with the mouse event data.
 */
void handle_mouse_event(struct packet pp);

/**
 * @brief Handles the falling of a piece in the game.
 */
void piece_fall();

/**
 * @brief Frees the array of scores.
 * 
 * @param scores The pointer to the array of scores.
 */
void free_array(int **scores);

#endif // INTERRUPT_MANAGER_H
