/* include/hardware/memory.h
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#include "hardware/hardware.h"

/* 6502 has a 16-bit address bus => 64KB (0x10000 bytes) memory */
#define MEMORY_SIZE 0x10000

/* physical ram: all address and data values are handled as unsigned
 * integers; format them as hex when logging or printing */
typedef struct {
	Hardware hardware;
	/* memory address register: 16-bit (0x0000 -> 0xFFFF) */
	uint16_t mar;
	/* memory data register: 8-bit (0x00 -> 0xFF) */
	uint8_t mdr;
	/* pending bus actions are consumed on memory_pulse only */
	int read_pending;
	int write_pending;
	uint64_t cycle;
	/* linear array of entire address space */
	unsigned char data[MEMORY_SIZE];
} Memory;

int memory_init(Memory *memory);
void memory_reset(Memory *memory);
void memory_set_mar(Memory *memory, uint16_t addr);
void memory_set_mdr(Memory *memory, uint8_t value);
uint16_t memory_get_mar(const Memory *memory);
uint8_t memory_get_mdr(const Memory *memory);
uint64_t memory_get_cycle(const Memory *memory);
uint8_t memory_peek(const Memory *memory, uint16_t addr);
void memory_request_read(Memory *memory);
void memory_request_write(Memory *memory);
int memory_is_busy(const Memory *memory);
/* latch mdr from data[mar] */
void memory_bus_read(Memory *memory);
/* store mdr into data[mar] */
void memory_bus_write(Memory *memory);
void memory_display(Memory *memory);
/* hex dump: one log line per row (16 bytes); start and length in hex semantics */
void memory_dump(Memory *memory, uint16_t start, uint16_t length);
void memory_pulse(void *context);

#endif // MEMORY_H
