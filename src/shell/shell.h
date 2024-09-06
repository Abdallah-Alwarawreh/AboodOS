#pragma once

#include <stdbool.h>
#include <stddef.h>

void shell_initialize(void);
void user_input(char c);
void execute_command(const char* command);
int compare_strings(const char* str1, const char* str2);
int starts_with(const char* str, const char* prefix);
