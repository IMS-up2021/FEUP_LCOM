#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

extern bool mouse_ready;
extern struct packet pp;

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
int (mouse_get_status)(uint8_t *st);
int (mouse_write_cmd)(uint8_t cmdb);
int (mouse_read_buffer)(uint8_t port, uint8_t *output);
int (mouse_disable_data_reporting)();
void (mouse_ih)();

#endif // MOUSE_H

