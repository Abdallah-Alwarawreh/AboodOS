#include "mouse.h"
#include "utils/irq.h"
#include "utils/util.h"

#define MOUSE_IRQ 12
#define MOUSE_PORT 0x60
#define MOUSE_STATUS_PORT 0x64
#define MOUSE_COMMAND_PORT 0x64

static int mouse_x = 0;
static int mouse_y = 0;

static void mouse_wait(unsigned char type) {
    unsigned int timeout = 100000;
    if (type == 0) {
        while (timeout--) {
            if ((inportb(MOUSE_STATUS_PORT) & 1) == 1) {
                return;
            }
        }
    } else {
        while (timeout--) {
            if ((inportb(MOUSE_STATUS_PORT) & 2) == 0) {
                return;
            }
        }
    }
}

static void mouse_write(unsigned char data) {
    mouse_wait(1);
    outportb(MOUSE_COMMAND_PORT, 0xD4);
    mouse_wait(1);
    outportb(MOUSE_PORT, data);
}

static unsigned char mouse_read() {
    mouse_wait(0);
    return inportb(MOUSE_PORT);
}

static void mouse_handler(struct Registers *regs) {
    static unsigned char mouse_cycle = 0;
    static char mouse_bytes[3];

    switch (mouse_cycle) {
        case 0:
            mouse_bytes[0] = mouse_read();
            mouse_cycle++;
            break;
        case 1:
            mouse_bytes[1] = mouse_read();
            mouse_cycle++;
            break;
        case 2:
            mouse_bytes[2] = mouse_read();
            mouse_x += mouse_bytes[1];
            mouse_y -= mouse_bytes[2];
            mouse_cycle = 0;
            break;
    }
}

void mouse_init() {
    unsigned char status;

    mouse_wait(1);
    outportb(MOUSE_COMMAND_PORT, 0xA8);

    mouse_wait(1);
    outportb(MOUSE_COMMAND_PORT, 0x20);
    mouse_wait(0);
    status = (inportb(MOUSE_PORT) | 2);
    mouse_wait(1);
    outportb(MOUSE_COMMAND_PORT, 0x60);
    mouse_wait(1);
    outportb(MOUSE_PORT, status);

    mouse_write(0xF6);
    mouse_read();

    mouse_write(0xF4);
    mouse_read();

    irq_install(MOUSE_IRQ, mouse_handler);
}
