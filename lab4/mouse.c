#include "mouse.h"

static int hook_id = 12;

struct packet pp;
int i = 0;
bool ready = false;

static enum state current_state = INITIAL;
int16_t delta_x = 0, delta_y = 0;

int(mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;

  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int(mouse_get_status)(uint8_t *st) {
  return util_sys_inb(STATUS_REG, st);
}

int(mouse_read_buffer)(uint8_t port, uint8_t *output) {
  int timeout = 0;
  uint8_t st;

  while (timeout < 3) {
    if (mouse_get_status(&st) != OK) {
      printf("Error getting status while reading from buffer\n");
      return -1;
    }

    if ((st & (OBF | AUX)) && !(st & (PARITY | TIME_OUT))) {
      if (util_sys_inb(port, output) != 0) {
        printf("error reading output from buffer\n");
        return -1;
      }
      return 0;
    }
    else
      return -1;

    tickdelay(micros_to_ticks(DELAY_US));
    timeout++;
  }
  return -1;
}

int(mouse_write_cmd)(uint8_t cmd) {
  int timeout = 0;
  uint8_t st;

  while (timeout < 3) {
    if (mouse_get_status(&st) != OK) {
      printf("error getting status while issuing command\n");
      return -1;
    }

    if ((st & IBF) == OK) {
      sys_outb(MOUSE_CMD_REG, WRITE_AUX);
    }

    if (mouse_get_status(&st) != 0) {
      printf("error getting status while issuing command\n");
      return -1;
    }

    if ((st & IBF) == OK) {
      sys_outb(MOUSE_ARGS, cmd);
    }

    tickdelay(micros_to_ticks(DELAY_US));

    uint8_t ack_byte;
    util_sys_inb(OUT_BUF, &ack_byte);

    if (ack_byte == MOUSE_OK)
      return 0;

    timeout++;
  }

  return -1;
}

int(my_mouse_enable_data_reporting)() {
  if (mouse_write_cmd(ENBL_DATA_REP) != OK) {
    printf("error enabling data report\n");
    return -1;
  }
  return 0;
}

int(mouse_disable_data_reporting)() {
  if (mouse_write_cmd(DIS_DATA_REP) != OK) {
    printf("error disabling data report\n");
    return -1;
  }
  return 0;
}

int(mouse_enable_stream_mode)() {
  if (mouse_write_cmd(SET_STREAM) != OK) {
    printf("error enabling stream mode\n");
    return -1;
  }
  return 0;
}

int(mouse_enable_remote_mode)() {
  if (mouse_write_cmd(SET_REMOTE) != OK) {
    printf("error enabling stream mode\n");
    return -1;
  }
  return 0;
}

void(mouse_ih)() {
    uint8_t output, st;
    mouse_get_status(&st);

    if((st & OBF) && !(st & (PARITY | TIME_OUT))){
        mouse_read_buffer(OUT_BUF, &output);
        pp.bytes[i] = output;

        if(i == 0 && !(output & FIRST_BYTE)){
            printf("BAD BYTE\n");
            return;
        }

        switch(i){
            case 0:
                pp.lb = output & BIT(0);
                pp.rb = output & BIT(1);
                pp.mb = output & BIT(2);

                pp.x_ov = output & BIT(6);
                pp.y_ov = output & BIT(7);
                break;
            case 1:
                pp.delta_x = output;
                if(pp.bytes[0] & BIT(4))
                    pp.delta_x |= 0xFF00;
                break;
            case 2:
                pp.delta_y = output;
                if(pp.bytes[0] & BIT(5))
                    pp.delta_y |= 0xFF00;
                break;
            default:
                break;
        }

        i++;

        if(i == 3){
            ready = true;
            i = 0;
        }
    }
}

bool (mouse_handle_gesture) (struct mouse_ev* event, uint8_t x_len, uint8_t tolerance){
  switch(current_state){
    case INITIAL: 
      delta_x = 0;
      delta_y = 0;
      if(event->type == LB_PRESSED){
        current_state = FIRST_SLOPE;
      }
      break;
    case FIRST_SLOPE:
      if(event->type == LB_RELEASED){
        if(delta_x >= x_len && delta_y/delta_x > 1){
          delta_x = 0;
          delta_y = 0;
          current_state = VERTEX;
        }
        else{
          current_state = INITIAL; 
        }
      }
      else if(event->type == MOUSE_MOV){
        if(event->delta_x <= 0 || event->delta_y <= 0){
          if(abs(event->delta_x) > tolerance || abs(event->delta_y) > tolerance){
            current_state = INITIAL;
          }
          break;
        }
        if(event->delta_x != 0 && (event->delta_y/event->delta_x <= 1))
          current_state = INITIAL;
        else{
          delta_x += event->delta_x;
          delta_y += event->delta_y;
        }
      }
      else
        current_state = INITIAL;
      break;
    case VERTEX:
      if(event->type == RB_PRESSED){
        if(abs(delta_x) > tolerance || abs(delta_y) > tolerance)
          current_state = INITIAL;
        else{
          delta_x = 0;
          delta_y = 0;
          current_state = SECOND_SLOPE;
        }
      }
      else if(event->type == MOUSE_MOV){
        delta_x += event->delta_x;
        delta_y += event->delta_y;
      }
      else if(event->type == LB_PRESSED){
        delta_x = 0;
        delta_y = 0;
        current_state = FIRST_SLOPE;
      }
      else{
        current_state = INITIAL;
      }
      break;
    case SECOND_SLOPE:
      if(event->type == RB_RELEASED){
        if(delta_x >= x_len && delta_y/delta_x < -1){
          current_state = COMPLETE;
          return true;
        }
        else {
          current_state = INITIAL;
        }
      }
      else if(event->type == MOUSE_MOV){
        if(event->delta_x <= 0 || event->delta_y >= 0){
          if(abs(event->delta_x) > tolerance || abs(event->delta_y) > tolerance){
            current_state = INITIAL;
          }
          break;
        }
        if(event->delta_x != 0 && (event->delta_y/event->delta_x) >= -1)
          current_state = INITIAL;
        else{
          delta_x += event->delta_x;
          delta_y += event->delta_y;
        }
      }
      else
        current_state = INITIAL;
      break;
    case COMPLETE:
      return true;
    default:
      break;      
  }    
  return false;
}
