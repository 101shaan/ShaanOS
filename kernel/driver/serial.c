#include <stdint.h>
#include "hal_driver.h"
#include "serial.h"

#define COM1_PORT 0x3F8

static inline void serial_out(uint16_t port, uint8_t value) {
	write_port(port, value);
}
static inline uint8_t serial_in(uint16_t port) {
	return read_port(port);
}

void serial_init(void) {
	// Disable interrupts
	serial_out(COM1_PORT + 1, 0x00);
	// Enable DLAB
	serial_out(COM1_PORT + 3, 0x80);
	// Set baud to 38400 (divisor 3)
	serial_out(COM1_PORT + 0, 0x03);
	serial_out(COM1_PORT + 1, 0x00);
	// 8 bits, no parity, one stop bit
	serial_out(COM1_PORT + 3, 0x03);
	// Enable FIFO, clear them, 14-byte threshold
	serial_out(COM1_PORT + 2, 0xC7);
	// IRQs enabled, RTS/DSR set
	serial_out(COM1_PORT + 4, 0x0B);
}

static int serial_tx_ready(void) {
	return (serial_in(COM1_PORT + 5) & 0x20) != 0;
}

void serial_write_char(char c) {
	if (c == '\n') {
		serial_write_char('\r');
	}
	while (!serial_tx_ready()) {}
	serial_out(COM1_PORT, (uint8_t)c);
}

void serial_write(const char* s) {
	while (*s) serial_write_char(*s++);
}


