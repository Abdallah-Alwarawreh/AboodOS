#ifndef ISR_H
#define ISR_H

#include "util.h"

struct Registers {
    unsigned int __ignored, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_no;
    unsigned int eip, cs, efl, useresp, ss;
};

void isr_install(unsigned int i, void (*handler)(struct Registers*));
void isr_init();

#endif
