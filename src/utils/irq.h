#ifndef IRQ_H
#define IRQ_H

#include "util.h"
#include "isr.h"

void irq_install(unsigned int i, void (*handler)(struct Registers*));
void irq_init();

#endif
