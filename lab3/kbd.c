#include "kbd.h"
#include "utils.h"

int hook_id = 1;
int counter = 0, i = 0;
bool ih_flag;
uint8_t scancode[2];


int (kbd_subscribe_int)(uint8_t *bit_no) {
    *bit_no = hook_id;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (kbd_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook_id);
}

int check_status(uint8_t st) {
    if (st & PARITY)
        return PARITY_ERR;

    if (st & TIME_OUT)
        return TIME_OUT_ERR;

    if (st & AUX)
        return AUX_DATA;

    return OK;
}

int (kbd_get_status)(uint8_t *st) {
    counter++;
    return util_sys_inb(STATUS_REG, st);
}

int (kbd_read_out_buffer)(uint8_t *output) {
    counter++;
    return util_sys_inb(OUT_BUF, output);
}

void (kbc_ih)() {
    uint8_t output;
    uint8_t status;

    if (kbd_read_out_buffer(&output) != OK)
        return;

    if (kbd_get_status(&status) != OK)
        return;

    if (check_status(status) == OK) {
        scancode[i] = output;
        ih_flag = (output != TWO_BYTE_SC) && (output & IS_BREAK);
    }
}

int (kbd_reenable_ints)() {
    uint8_t output;

    if (kbd_write_cmd(KBC_CMD_REG, READ_CMDB) != 0) {
        printf("Error writing command byte while reenabling interrupts\n");
        return -1;
    }

    if (kbd_read_ret_cmd(&output) != 0) {
        printf("Error reading return command byte\n");
        return -1;
    }

    output |= KBC_INT;

    if (kbd_write_cmd(KBC_CMD_REG, WRITE_CMDB) != 0) {
        printf("Error writing command byte while reenabling interrupts\n");
        return -1;
    }

    if (kbd_write_cmd(KBC_ARGS, output) != 0) {
        printf("Error writing command byte while reenabling interrupts\n");
        return -1;
    }

    return 0;
}

int (kbd_write_cmd)(int port, uint8_t cmd) {
    int timeout = 0;
    uint8_t st;

    while (timeout < 3) {
        if (kbd_get_status(&st) != 0) {
            printf("Error getting status while issuing command\n");
            return -1;
        }

        if ((st & IBF) == 0) {
            sys_outb(port, cmd);
            return 0;
        }

        tickdelay(micros_to_ticks(DELAY_US));
        timeout++;
    }

    return -1;
}

int (kbd_read_ret_cmd)(uint8_t *data) {
    int timeout = 0;
    uint8_t st;

    while (timeout < 3) {
        if (kbd_get_status(&st) != 0) {
            printf("Error getting status when reading command byte return value\n");
            return -1;
        }

        if (st & OBF && !(st & (PARITY | TIME_OUT | AUX))) {
            if (kbd_read_out_buffer(data) != 0) {
                printf("Error reading KBC command byte return value from output buffer\n");
                return -1;
            }
            return 0;
        } else {
            return -1;
        }

        tickdelay(micros_to_ticks(DELAY_US));
        timeout++;
    }

    return -1;
}
