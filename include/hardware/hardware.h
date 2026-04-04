/* include/hardware/hardware.h
 *
 * by: thomas bruce
 * date: 27/01/2026
 */

#ifndef HARDWARE_H
#define HARDWARE_H

#include <stddef.h>

/* base structure for all hardware components */
typedef struct {
	int id;
	char *name;
	int debug;
} Hardware;

void hardware_init(Hardware *hw, int id, const char *name);
void hardware_log(Hardware *hw, const char *message);
/* writes zero-padded uppercase hex into buf, null-terminated
 * should be safe for concurrent use now when each caller uses its own buf
 * -1 if buf is invalid or output is trunc
 */
int hexLog(char *buf, size_t buflen, unsigned int number, int length);

#endif // HARDWARE_H
