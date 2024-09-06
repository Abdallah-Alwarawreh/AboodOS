#ifndef IDT_H
#define IDT_H

#include "util.h"
#include "isr.h"

void idt_set(unsigned char index, void (*base)(struct Registers*), unsigned short selector, unsigned char flags);
void idt_init();

#endif
