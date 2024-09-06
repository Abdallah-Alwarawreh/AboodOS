#include "idt.h"

struct IDTEntry {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char __ignored;
    unsigned char type;
    unsigned short offset_high;
} PACKED;

struct IDTPointer {
    unsigned short limit;
    unsigned int base;
} PACKED;

static struct {
    struct IDTEntry entries[256];
    struct IDTPointer pointer;
} idt;

// in start.S
extern void idt_load();

void idt_set(unsigned char index, void (*base)(struct Registers*), unsigned short selector, unsigned char flags) {
    idt.entries[index] = (struct IDTEntry) {
        .offset_low = ((unsigned int) base) & 0xFFFF,
        .offset_high = (((unsigned int) base) >> 16) & 0xFFFF,
        .selector = selector,
        .type = flags | 0x60,
        .__ignored = 0
    };
}

void idt_init() {
    idt.pointer.limit = sizeof(idt.entries) - 1;
    idt.pointer.base = (unsigned int) &idt.entries[0];
    memset(&idt.entries[0], 0, sizeof(idt.entries));
    idt_load((unsigned int) &idt.pointer);
}
