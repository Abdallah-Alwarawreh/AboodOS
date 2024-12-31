#ifndef MOUSE_H
#define MOUSE_H

void mouse_init();
void mouse_handler(struct Registers *regs);
unsigned char mouse_read();

#endif
