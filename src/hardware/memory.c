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

void memory_init(Memory *memory) {
	/* register this memory instance with the hardware logging system */
	hardware_init(&memory->hardware, 0, "RAM");

	/* zero out all memory locations on startup */
	for (unsigned int i = 0x0000; i < 0x10000; i++) {
		memory->data[i] = 0x00;
	}
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

void memory_pulse(void *context) {
	Memory *memory = (Memory *) context;
	hardware_log(&memory->hardware, "received clock pulse");
}
