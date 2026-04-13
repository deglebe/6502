/* src/hardware/memory.c
 * memory operations
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware/memory.h"

int memory_init(Memory *memory) {
	if (hardware_init(&memory->hardware, 0, "RAM") != 0) {
		return -1;
	}
	char message[128];
	snprintf(message, sizeof(message), "Created - Addressable space : %u", MEMORY_SIZE);
	hardware_log(&memory->hardware, message);

	memory_reset(memory);
	return 0;
}

void memory_reset(Memory *memory) {
	memory->mar = 0x0000;
	memory->mdr = 0x00;

	/* zero out all memory locations */
	for (unsigned int i = 0x0000; i < 0x10000; i++) {
		memory->data[i] = 0x00;
	}
}

void memory_set_mar(Memory *memory, uint16_t addr) {
	memory->mar = addr;
}

void memory_set_mdr(Memory *memory, uint8_t value) {
	memory->mdr = value;
}

uint16_t memory_get_mar(const Memory *memory) {
	return memory->mar;
}

uint8_t memory_get_mdr(const Memory *memory) {
	return memory->mdr;
}

void memory_bus_read(Memory *memory) {
	memory->mdr = memory->data[memory->mar];
}

void memory_bus_write(Memory *memory) {
	memory->data[memory->mar] = memory->mdr;
}

void memory_display(Memory *memory) {
	char message[256];
	char addr_str[32];
	char value_str[32];

	/* display the first 21 bytes (0x00-0x14) */
	for (unsigned int addr = 0x00; addr <= 0x14; addr++) {
		if (addr < MEMORY_SIZE) {
			unsigned char value = memory->data[addr];
			if (hexLog(addr_str, sizeof(addr_str), addr, 5) != 0
				|| hexLog(value_str, sizeof(value_str), value, 2) != 0) {
				hardware_log(&memory->hardware,
					"Address : ERR Contains value: ERR [hexValue conversion]: number undefined");
			}
			else {
				snprintf(message,
					sizeof(message),
					"Address : %s Contains value: %s",
					addr_str,
					value_str);
				hardware_log(&memory->hardware, message);
			}
		}
		else {
			if (hexLog(addr_str, sizeof(addr_str), addr, 5) != 0) {
				hardware_log(&memory->hardware,
					"Address : ERR Contains value: ERR [hexValue conversion]: number undefined");
			}
			else {
				snprintf(message,
					sizeof(message),
					"Address : %s Contains value: ERR [hexValue "
					"conversion]: number undefined",
					addr_str);
				hardware_log(&memory->hardware, message);
			}
		}
	}
}

void memory_dump(Memory *memory, uint16_t start, uint16_t length) {
	char line[320];
	char byte_str[8];

	if (memory == NULL || length == 0) {
		return;
	}

	for (uint32_t off = 0; off < (uint32_t)length; off += 0x10) {
		uint16_t row_base = (uint16_t)(start + (uint16_t)off);
		char addr_head[8];
		if (hexLog(addr_head, sizeof(addr_head), row_base, 4) != 0) {
			hardware_log(&memory->hardware, "[dump] ERR: address format");
			continue;
		}
		int pos = snprintf(line, sizeof(line), "[dump] 0x%s:", addr_head);
		if (pos < 0 || (size_t)pos >= sizeof(line)) {
			continue;
		}

		uint32_t row_len = 0x10;
		if (off + row_len > (uint32_t)length) {
			row_len = (uint32_t)length - off;
		}
		for (uint32_t i = 0; i < row_len; i++) {
			uint16_t addr = (uint16_t)(row_base + (uint16_t)i);
			uint8_t v = memory->data[addr];
			if (hexLog(byte_str, sizeof(byte_str), v, 2) != 0) {
				continue;
			}
			int n = snprintf(line + pos, sizeof(line) - (size_t)pos, " %s", byte_str);
			if (n < 0 || (size_t)(pos + n) >= sizeof(line)) {
				break;
			}
			pos += n;
		}
		hardware_log(&memory->hardware, line);
	}
}

void memory_pulse(void *context) {
	Memory *memory = (Memory *) context;
	hardware_log(&memory->hardware, "received clock pulse");
}
