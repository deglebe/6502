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

int hardware_init(Hardware *hw, int id, const char *name) {
	if (hw == NULL || name == NULL) {
		return -1;
	}
	hw->id = id;
	hw->name = NULL;
	hw->debug = 1;
	size_t len = strlen(name) + 1;
	char *p = (char *) malloc(len);
	if (p == NULL) {
		return -1;
	}
	memcpy(p, name, len);
	hw->name = p;
	return 0;
}

void hardware_log(Hardware *hw, const char *message) {
	/* only log if debug is enabled */
	if (hw != NULL && hw->debug) {
		const char *label = (hw->name != NULL) ? hw->name : "(unnamed)";
		long timestamp = (long) time(NULL);
		printf("[hw - %s id: %d - %ld]: %s\n", label, hw->id, timestamp, message);
	}
}

int hexLog(char *buf, size_t buflen, unsigned int number, int length) {
	if (buf == NULL || buflen == 0) {
		return -1;
	}
	/* %0*X: zero-pad to width length, uppercase hex */
	int n = snprintf(buf, buflen, "%0*X", length, number);
	if (n < 0 || (size_t) n >= buflen) {
		return -1;
	}
	return 0;
}
