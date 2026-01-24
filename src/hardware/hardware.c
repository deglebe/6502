/* src/hardware/hardware.c
 * hardware generics
 *
 * by: thomas bruce
 * date: 24/01/2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hardware/hardware.h"

void hardware_init(Hardware *hw, int id, const char *name) {
	hw->id = id;
	hw->name = (char *) malloc(strlen(name) + 1);
	strcpy(hw->name, name);
	hw->debug = 1;
}

void hardware_log(Hardware *hw, const char *message) {
	if (hw->debug) {
		long timestamp = (long) time(NULL);
		printf("[hw - %s id: %d - %ld]: %s\n", hw->name, hw->id, timestamp, message);
	}
}
