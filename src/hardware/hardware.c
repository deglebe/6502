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

int hexLog(char *buf, size_t buflen, unsigned int number, int length) {
	if (buf == NULL || buflen == 0) {
		return -1;
	}
	/* %0*X: zero-pad to width length, uppercase hex */
	int n = snprintf(buf, buflen, "%0*X", length, number);
	if (n < 0 || (size_t)n >= buflen) {
		return -1;
	}
	return 0;
}
