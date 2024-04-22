#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include <stdbool.h>

#include "i8042.h"
#include "utils.h"

#define DELAY_US 25000
#define PREPARE_CMD 0xD4
#define MOUSE_FORWARD 0xD4 

enum state{
  INITIAL,
  FIRST_SLOPE,
  VERTEX,
  SECOND_SLOPE,
  COMPLETE
};


#define OK              0
#define PARITY_ERR      1
#define TIME_OUT_ERR    2
#define IBF_FULL        3
#define AUX_DATA        4

int (mouse_subscribe_int) (uint8_t* bit_no);
int (mouse_unsubscribe_int)();
int (my_mouse_enable_data_reporting)();
int (mouse_disable_data_reporting)();
int (mouse_enable_stream_mode)();
int (mouse_enable_remote_mode)();
int (mouse_get_status) (uint8_t* st);
int (mouse_read_buffer) (uint8_t port, uint8_t* output);
int (mouse_write_cmd) (uint8_t cmdb);
void (mouse_ih)();
bool (mouse_handle_gesture) (struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

#endif 
