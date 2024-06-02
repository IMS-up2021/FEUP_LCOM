#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#define DELAY_US 20000 // Tick delay

#define PARITY_ERR   1
#define TIME_OUT_ERR 2
#define IBF_FULL     3
#define AUX_DATA     4

extern uint8_t scan_code[2];
extern int kdb_i;

int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();
int (check_status)(uint8_t st);
int (kbd_get_status)(uint8_t *st);
int (kbd_read_out_buffer)(uint8_t *output);
int (kbd_write_cmdb)(int port, uint8_t cmdb);
int (kbd_read_ret_cmdb)(uint8_t *data);
int (kbd_reenable_ints)();
void (kbc_ih)();

#endif // KEYBOARD_H
