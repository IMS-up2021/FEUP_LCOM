#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include <kbd.h>

extern int counter;
extern bool makecode;
extern uint8_t scancode[2];

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g5/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g5/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (kbd_test_scan)() {
  uint8_t bit_no;

  if (kbd_subscribe_int(&bit_no) != OK)
    return 1;

  uint32_t irq_set = BIT(bit_no);

  message msg;
  int ipc_status;
  int index = 0;

  while (scancode[index] != ESC_BREAK) {
    int receive_result;

    if ((receive_result = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with : %d", receive_result);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: {
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();

            if (scancode[index] == TWO_BYTE_SC) {
              index++;
              continue;
            }

            kbd_print_scancode(makecode, (index + 1), scancode);
            index = 0;
          }
          break;
        }
        default:
          break;
      }
    }
  }

  if (kbd_unsubscribe_int() != OK)
    return 1;

  kbd_print_no_sysinb(counter);

  return 0;
}

int(kbd_test_poll)() {
  int i = 0;
  while(scancode[i] != ESC_BREAK){
    uint8_t st;
    if(kbd_get_status(&st) != OK){
      printf("Error ocurred\n");
      return -1;
    }
    if(st & OBF){
      kbc_ih();
      if(scancode[i] == TWO_BYTE_SC){
        i++;
        continue;
      }
      kbd_print_scancode(makecode, (i+1), scancode);
      i = 0;
    }
  }
  kbd_print_no_sysinb(counter);
  return 0;
}


int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t bit_no_timer;
  uint8_t bit_no_KBC;
  timer_subscribe_int(&bit_no_timer);
  kbd_subscribe_int(&bit_no_KBC);
  int index = 0;
  uint32_t irq_set_kbd = BIT(bit_no_KBC);
  uint32_t irq_set_timer = BIT(bit_no_timer);
  message msg;
  uint8_t count = 0;
  int ipc_status;
  while (scancode[index] != ESC_BREAK && n != count){
    int receive_result = driver_receive(ANY, &msg, &ipc_status);
    if (receive_result != OK) {
      printf("driver_receive failed with : %d", receive_result);
      continue;
    }
    if (is_ipc_notify(ipc_status)){
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: {
          if (msg.m_notify.interrupts & irq_set_kbd) {
            kbc_ih();
            if (scancode[index] == TWO_BYTE_SC) {
              index++;
              continue;
            }
            kbd_print_scancode(makecode, (index + 1), scancode);
            index = 0;
            counter = 0;
            count = 0;
          }
          if (msg.m_notify.interrupts & irq_set_timer){
            counter++;
            if (counter % 60 == 0){
              timer_print_elapsed_time();
              count++;
            }
          }
          break;
        }  
        default:
          break;
      }  
    }
  }
  kbd_unsubscribe_int();
  timer_unsubscribe_int();
  kbd_print_no_sysinb(counter);
  return 0;
}
