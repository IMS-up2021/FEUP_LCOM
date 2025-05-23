#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

int timer_counter = 0;
static int hook_id = 0;

// change the frequency of any timer
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint32_t rb_cmd = TIMER_RB_CMD | BIT(5) | TIMER_RB_SEL(timer);

  sys_outb(TIMER_CTRL, rb_cmd);

  uint8_t config = 0;

  timer_get_conf(timer, &config);

  config &= TIMER_M_MASK;
  config |= TIMER_LSB_MSB;

  switch(timer) {
    case 0: config |= TIMER_SEL0; break;
    case 1: config |= TIMER_SEL1; break;
    case 2: config |= TIMER_SEL2; break;
    default: return 1;
  }

  sys_outb(TIMER_CTRL, config);

  uint16_t new_clock = TIMER_FREQ/freq;

  uint8_t lsb = 0, msb = 0;

  util_get_LSB(new_clock, &lsb);
  util_get_MSB(new_clock, &msb);

  switch(timer){
    case 0: sys_outb(TIMER_0, lsb); sys_outb(TIMER_0, msb); break;
    case 1: sys_outb(TIMER_1, lsb); sys_outb(TIMER_1, msb); break;
    case 2: sys_outb(TIMER_2, lsb); sys_outb(TIMER_2, msb); break;
    default: return 1;
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;
  return sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);;
}

void (timer_int_handler)() {
  timer_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  switch(timer) {
    case 0: return util_sys_inb(TIMER_0, st);
    case 1: return util_sys_inb(TIMER_1, st);
    case 2: return util_sys_inb(TIMER_2, st);
    default: return 1;
  }
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val val;

  switch(field) {
    case tsf_all: 
      val.byte = st;
      return timer_print_config(timer, field, val);

    case tsf_initial:
      val.in_mode = ((st & (BIT(4) | BIT(5))) >> 4);
      return timer_print_config(timer, field, val);

    case tsf_mode:
      val.count_mode = ((st & (BIT(1) | BIT(2) | BIT(3))) >>1);
      return timer_print_config(timer, field, val);

    case tsf_base:
      val.bcd = ((st & BIT(0)));
      return timer_print_config(timer, field, val);

    default: return 1;
  }
}

