#include "vga.h"
#include "keyboard.h"
#include "utils/util.h"
#include "utils/idt.h"
#include "utils/isr.h"
#include "utils/irq.h"
#include "utils/timer.h"
#include "utils/util.h"

#define FPS 30

void kernel_main(void) {
	idt_init();
    isr_init();
    irq_init();
	terminal_initialize();
	timer_init();
    keyboard_init();

char* AboodOS = "\n"
"     _    _                     _    ___  ____  \n"
"    / \\  | |__   ___   ___   __| |  / _ \\/ ___| \n"
"   / _ \\ | '_ \\ / _ \\ / _ \\ / _` | | | | \\___ \\ \n"
"  / ___ \\| |_) | (_) | (_) | (_| | | |_| |___) |\n"
" /_/   \\_\\_.__/ \\___/ \\___/ \\__,_|  \\___/|____/ \n"
"\n"
"By abdallahdev\n"
"Version 0.0.1\n"
"\n";
                                          

	terminal_writestring(AboodOS);
	terminal_writestring("We got Abood OS before gta6\n");
	terminal_writestring("");


    unsigned int last_frame = 0, last = 0, last_key_press = 0;
	char LastKeys[128] = { 0 };

	while(true) {
		unsigned int x,y;
		terminal_getcursorposition(&x, &y);
		const unsigned int now = (unsigned int) timer_get();

        if (now != last) {
            last = now;
        }

		if ((now - last_frame) > (TIMER_TPS / FPS)) {
            last_frame = now;

			// Check if the last key pressed was released
			bool Keys[128] = { 0 };
			
			for (int i = 0; i < 128; i++) {
				if (keyboard.chars[i]) {
					Keys[i] = true;
				}else if (keyboard.releasedchars[i]) {
					Keys[i] = false;
				}
			}

			for (int i = 0; i < 128; i++) {
				if (LastKeys[i] != Keys[i]) {
					if (Keys[i]) {
						// DEBUGGING
						// char snum[5];
						// itoa(i, snum, 10);
						// terminal_writestring(snum);

						if (i == 0) continue;

						if ((char) i == KEY_BACKSPACE) {
							terminal_backspace();
						}else if ((char) i == KEY_ENTER) {
							terminal_writestring("\n");
						}else {
							bool Shift = keyboard.mods & KEY_MOD_SHIFT;
							char c = (char) i;

							if (Shift)
								c = shift_char(c);
							
							terminal_putchar(c);
						}
					}
				}

				LastKeys[i] = Keys[i];
			}
		}
		
	}
}