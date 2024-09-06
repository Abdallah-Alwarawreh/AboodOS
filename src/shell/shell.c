#include "shell.h"
#include "vga.h"

#define MAX_INPUT_SIZE 256

static char input_buffer[MAX_INPUT_SIZE];
static size_t input_position = 0;

#define amogus "\n"\
"               AAAAAAAAAAAA            \n"\
"             AA            AA          \n"\
"            AA               A         \n"\
"           AA       AAAAAAAAAAAA       \n"\
"          AA     AA            AAA     \n"\
"       AAAAA     A              AA     \n"\
"    AAA   A      A               A     \n"\
"    AA    A       A             AA     \n"\
"   AA     A        AAAAAAAAAAAAAA      \n"\
"   AA     A                    AA      \n"\
"   AA     A                    AA      \n"\
"   AA     A                    A       \n"\
"   AA     A                    A       \n"\
"   AA     A                   AA       \n"\
"   AA     A                   AA       \n"\
"    AA    A                   AA       \n"\
"     AAAAAA         AAA       A        \n"\
"          A        AA AA     AA        \n"\
"          AA       AA AA     AA        \n"\
"           AAAAAAAAA   AAAAAAA         \n"

#define flappy "\n"  \
"              FFFFFFFFFFFFF           \n" \
"         FFFFF      FF     FF         \n" \
"       FF         FF         FF       \n" \
"   FFFFFFF        FF         FFFF     \n" \
"   FFFFFFFF       FF       FF  FF     \n" \
"FFF        FFF    FF       FF  FF     \n" \
"FFF           FF    FF         FF     \n" \
"FFF           FF      FFFFFFFFFFFFFF  \n" \
"   FF      FFF      FFF            FFF\n" \
"    FFFFFFFF      FFFFFFFFFFFFFFFFFF  \n" \
"     FFFFFF       FFFFFFFFFFFFFFFFFF  \n" \
"     FF             FFFFFFFFFFFFFFFF  \n" \
"       FFFFF          FFFFFFFFFFF     \n" \
"           FFFFFFFFFFF                \n"

#define AboodOS "\n" \
"     _    _                     _    ___  ____  \n" \
"    / \\  | |__   ___   ___   __| |  / _ \\/ ___| \n" \
"   / _ \\ | '_ \\ / _ \\ / _ \\ / _` | | | | \\___ \\ \n" \
"  / ___ \\| |_) | (_) | (_) | (_| | | |_| |___) |\n" \
" /_/   \\_\\_.__/ \\___/ \\___/ \\__,_|  \\___/|____/ \n" \
"\n" \
"By abdallahdev\n" \
"Version 0.0.1\n" \
"\n"

void shell_initialize(void) {
    terminal_initialize();
    terminal_writestring(AboodOS);
    terminal_writestring("AboodOS Shell Initialized\n");
    terminal_writestring("> ");
}

void user_input(char c) {
    if (c == '\n') {
        input_buffer[input_position] = '\0';
        execute_command(input_buffer);
        input_position = 0;
        int x, y;
        terminal_getcursorposition(&x, &y);
        
        terminal_writestring("> ");
    } else if (c == '\b') {
        if (input_position > 0) {
            terminal_backspace();
            input_position--;
        }
    } else if (input_position < MAX_INPUT_SIZE - 1) {
        input_buffer[input_position++] = c;
        terminal_putchar(c);
    }
}

void execute_command(const char* command) {
    if (compare_strings(command, "clear") == 0) {
        terminal_initialize(); 
    } else if (starts_with(command, "echo ")) {
        terminal_writestring("\n");
        terminal_writestring(command + 5);
        terminal_writestring("\n");
    } else if (compare_strings(command, "amogus") == 0) {
        terminal_writestring(amogus);
    } else if (compare_strings(command, "flappy") == 0) {
        terminal_writestring(flappy);
    } else if (compare_strings(command, "") == 0){
        terminal_writestring("\n");
    } else {
        terminal_writestring("\n");
        terminal_writestring("Unknown command: ");
        terminal_writestring(command);
        terminal_writestring("\n");
    }
}

int compare_strings(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int starts_with(const char* str, const char* prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return 0;
        }
    }
    return 1;
}
