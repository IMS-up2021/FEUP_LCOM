#ifndef _KBD_H_
#define _KBD_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "../lab3/i8042.h"

#define DELAY_US 20000

#define PARITY_ERR   1
#define TIME_OUT_ERR 2
#define IBF_FULL     3
#define AUX_DATA     4

#define IS_BREAKCODE 1
#define IS_MAKECODE 2
#define IH_STATUS_ERROR 5

int (kbd_subscribe_int) (uint8_t* bit_no);
int (kbd_unsubscribe_int) ();
int (check_status) (uint8_t st);
int (kbd_get_status) (uint8_t* st);
int (kbd_read_out_buffer) (uint8_t* output);
int (kbd_reenable_ints) ();
int (kbd_write_cmd) (int port, uint8_t cmd);
int (kbd_read_ret_cmd) (uint8_t* data);

#endif /*_KBD_H_*/
