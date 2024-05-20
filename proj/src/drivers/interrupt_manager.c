#include "interrupt_manager.h"

uint32_t timer_irq_set, kbd_irq_set, mouse_irq_set;

int(init)() {
    uint8_t timer_bit_no, kbd_bit_no, mouse_bit_no;

    // Subscribe to interrupts
    if (timer_subscribe_int(&timer_bit_no) != OK) {
        printf("Failed to subscribe to timer interrupts\n");
        return EXIT_FAILURE;
    }
    if (kbd_subscribe_int(&kbd_bit_no) != OK) {
        printf("Failed to subscribe to keyboard interrupts\n");
        timer_unsubscribe_int();
        return EXIT_FAILURE;
    }
    if (mouse_subscribe_int(&mouse_bit_no) != OK) {
        printf("Failed to subscribe to mouse interrupts\n");
        kbd_unsubscribe_int();
        timer_unsubscribe_int();
        return EXIT_FAILURE;
    }

    // Enable data reporting for mouse
    if (mouse_enable_data_reporting() != OK) {
        printf("Failed to enable mouse data reporting\n");
        mouse_unsubscribe_int();
        kbd_unsubscribe_int();
        timer_unsubscribe_int();
        return EXIT_FAILURE;
    }

    timer_irq_set = BIT(timer_bit_no);
    kbd_irq_set = BIT(kbd_bit_no);
    mouse_irq_set = BIT(mouse_bit_no);

    if (vg_init(VBE_600p_DC) == NULL) {
        printf("Failed to initialize video graphics mode\n");
        mouse_disable_data_reporting();
        mouse_unsubscribe_int();
        kbd_unsubscribe_int();
        timer_unsubscribe_int();
        return EXIT_FAILURE;
    }

    if (load_xpms() != OK) {
        printf("Failed to load XPMs\n");
        vg_exit();
        mouse_disable_data_reporting();
        mouse_unsubscribe_int();
        kbd_unsubscribe_int();
        timer_unsubscribe_int();
        return EXIT_FAILURE;
    }

    get_keys();
    get_scores();

    return EXIT_SUCCESS;
}

int(main_loop)() {
    message msg;
    int ipc_status, r;

    while (state != EXIT) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
            printf("driver_receive failed with: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_int_handler();

                        if (timer_counter % 1 == 0) {
                            handle_timer_event();
                        }

                        if (state == PLAYING && (timer_counter % falling_speed == 0)) {
                            piece_fall();
                        }
                    }
                    if (msg.m_notify.interrupts & kbd_irq_set) {
                        kbc_ih();

                        if (scan_code[kbd_i] == TWO_BYTE_SC) {
                            kbd_i++;
                            continue;
                        }

                        kbd_i = 0;
                        handle_kbd_event(scan_code);
                        scan_code[0] = 0;
                        scan_code[1] = 0;
                    }
                    if (msg.m_notify.interrupts & mouse_irq_set) {
                        mouse_ih();

                        if (mouse_ready) {
                            mouse_ready = false;
                            handle_mouse_event(pp);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    return EXIT_SUCCESS;
}

int(leave)() {
    if (timer_unsubscribe_int() != OK) {
        printf("Failed to unsubscribe timer interrupts\n");
    }
    if (kbd_unsubscribe_int() != OK) {
        printf("Failed to unsubscribe keyboard interrupts\n");
    }
    if (mouse_unsubscribe_int() != OK) {
        printf("Failed to unsubscribe mouse interrupts\n");
    }
    if (mouse_disable_data_reporting() != OK) {
        printf("Failed to disable mouse data reporting\n");
    }
    free_array(&scores);
    vg_exit();

    return EXIT_SUCCESS;
}
