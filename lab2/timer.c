#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

#include "utils.h"

#define TIMER(n) (TIMER_0 + n)
#define TIMER_SEL(n) (n << 6)

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint32_t control = TIMER_BIN | TIMER_SQR_WAVE | TIMER_LSB_MSB | TIMER_SEL(timer);
  int err = sys_outb(TIMER_CTRL, control);
  if (err) return err;

  err = sys_outb(TIMER(timer), freq & 0xFF);
  if (err) return err;

  err = sys_outb(TIMER(timer), (freq >> 8) & 0xFF);
  if (err) return err;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  int err = sys_outb(TIMER_CTRL, TIMER_RB_SEL(timer) |  TIMER_RB_COUNT_ | TIMER_RB_CMD);
  if (err) return err;

  err = util_sys_inb(TIMER(timer), st);
  if (err) return err;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  union timer_status_field_val value;

  switch (field) {
    case tsf_all:
      value.byte = st;
      break;
    case tsf_initial:
      value.in_mode = st & TIMER_LSB_MSB;
      break;
    case tsf_mode:
      value.count_mode = (st >> 1) & 7;
      break;
    case tsf_base:
      value.bcd = st & TIMER_BCD;
      break;
  }

  int err = timer_print_config(timer, field, value);
  if (err) return err;

  return 0;  
}
