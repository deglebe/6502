/* src/hardware/memory.c
 * memory operations
 *
 * by: thomas bruce
 * date: 27/01/2026
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
			/* format address as 5 digit hex, val as 2 digit hex
			 * copy to separate buffers */
			strcpy(addr_str, hexLog(addr, 5));
			strcpy(value_str, hexLog(value, 2));
			snprintf(message,
				sizeof(message),
				"Address : %s Contains value: %s",
				addr_str,
				value_str);
			hardware_log(&memory->hardware, message);
		}
		else {
			/* handle oob addresses */
			strcpy(addr_str, hexLog(addr, 5));
			snprintf(message,
				sizeof(message),
				"Address : %s Contains value: ERR [hexValue "
				"conversion]: number undefined",
				addr_str);
			hardware_log(&memory->hardware, message);
		}
	}
}
