// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mouse.h"
#include "timer.c"

extern struct packet pp;
extern int i;
extern bool ready;
extern int timer_counter;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/g5/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g5/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {

  uint8_t bit_no;

  if (my_mouse_enable_data_reporting() != OK) {
    printf("error enabling data reporting\n");
    return -1;
  }
    
  mouse_subscribe_int(&bit_no);

  uint32_t irq_set = BIT(bit_no);

  message msg;

  int ipc_status;

  while (cnt > 0) {
    int r;

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with : %d", r);
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: {
          if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();

            if (ready){
              ready = false;
              mouse_print_packet(&pp);
              cnt--;
            }
          }
          break;
        }
        default: break;
      }
    }
  }
  
  mouse_unsubscribe_int();
  mouse_disable_data_reporting();
  return 0;

}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t bit_no;
  uint8_t bit_no_timer;

  uint8_t count = 0;

  if(my_mouse_enable_data_reporting() != OK){
    printf("error enabling data reporting\n");
    return -1;
  }  

  mouse_subscribe_int(&bit_no);
  timer_subscribe_int(&bit_no_timer);

  uint32_t irq_set = BIT(bit_no);
  uint32_t irq_set_timer = BIT(bit_no_timer);


  message msg;

  int ipc_status;


  while(count != idle_time){
    int r;

    if((r = driver_receive(ANY, &msg, &ipc_status)) != OK){
      printf("driver_receive failed with : %d", r);
    }

    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: {
          if(msg.m_notify.interrupts & irq_set){
            mouse_ih();

            if(ready){
              ready = false;
              mouse_print_packet(&pp);
              count = 0;
              timer_counter = 0;
            }

          }

          if(msg.m_notify.interrupts & irq_set_timer){
            timer_int_handler();

            if(timer_counter % 60 == 0){
              timer_print_elapsed_time();
              count++;
            }

          }

          break;
        }
        default: break;
      }
    }
  }

  mouse_unsubscribe_int();
  mouse_disable_data_reporting();

  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t bit_no;

  if (my_mouse_enable_data_reporting() != OK) {
    printf("error enabling data reporting\n");
    return -1;
  }  

  mouse_subscribe_int(&bit_no);

  uint32_t irq_set = BIT(bit_no);

  struct mouse_ev* event;

  message msg;


  int ipc_status;
  bool event_handled = false;

  while (!event_handled) {
    int r;

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with : %d", r);
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: {
          if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();

            if(ready){
              ready = false;
              mouse_print_packet(&pp);

              event = mouse_detect_event(&pp);
              event_handled = mouse_handle_gesture(event, x_len, tolerance);
            }
          }
          break;
        }
        default: break;
      }
    }
  }

  mouse_unsubscribe_int();
  mouse_enable_remote_mode();
  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
