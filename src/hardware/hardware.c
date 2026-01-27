/* src/hardware/hardware.c
 * hardware generics
 *
 * by: thomas bruce
 * date: 27/01/2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hardware/hardware.h"

void hardware_init(Hardware *hw, int id, const char *name) {
	hw->id = id;
	/* allocate and copy name string for ident. persistence */
	hw->name = (char *) malloc(strlen(name) + 1);
	strcpy(hw->name, name);
	hw->debug = 1;
}

void hardware_log(Hardware *hw, const char *message) {
	/* only log if debug is enabled */
	if (hw->debug) {
		long timestamp = (long) time(NULL);
		printf("[hw - %s id: %d - %ld]: %s\n", hw->name, hw->id, timestamp, message);
	}
}

/* format number as uppercase hex with zero-padding
 * length parameter is min digits
 * caller should copy result if needed multiple times
 */
char *hexLog(unsigned int number, int length) {
	static char hex_buffer[32];
	/* %0*X: 0 = zero-pad, * = width from parameter, X = uppercase hex */
	snprintf(hex_buffer, sizeof(hex_buffer), "%0*X", length, number);
	return hex_buffer;
}
