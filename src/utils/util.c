#include "util.h"

char shift_char(char ch) {
    // Manual conversion from lowercase to uppercase
    if (ch >= 'a' && ch <= 'z') {
        return ch - ('a' - 'A');  // Subtracts the ASCII difference between lowercase and uppercase
    }

    // Conversion of digits to their corresponding symbols
    switch (ch) {
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        default: break;
    }

    // Conversion of special characters to their shifted counterparts
    switch (ch) {
        case '`': return '~';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        default: break;
    }

    // If the character does not change when Shift is pressed, return it unchanged
    return ch;
}