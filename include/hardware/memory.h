/* include/hardware/memory.h
 *
 * by: thomas bruce
 * date: 27/01/2026
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "hardware/hardware.h"

/* 6502 has a 16-bit address bus => 64KB (0x10000 bytes) memory */
#define MEMORY_SIZE 0x10000

typedef struct {
	Hardware hardware;
	/* linear array of entire address space */
	unsigned char data[MEMORY_SIZE];
} Memory;

void memory_init(Memory *memory);
void memory_display(Memory *memory);

#endif // MEMORY_H
