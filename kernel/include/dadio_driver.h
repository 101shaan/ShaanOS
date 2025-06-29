#ifndef DADIO_DRIVER_H
#define DADIO_DRIVER_H
#include <stdint.h>

// VGA color enum and function declarations as before

void monitor_puts(char* Message);
void printhex (uint32_t input);
void printint (uint32_t input);
void putc(char input);
void set_fg_color(int color);
void set_bg_color(int color);
void clear();
char get_monitor_char();
uint32_t get_cursor();
void set_cursor(uint32_t);

#endif
