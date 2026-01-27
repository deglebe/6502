/* include/hardware/hardware.h
 *
 * by: thomas bruce
 * date: 27/01/2026
 */

#ifndef HARDWARE_H
#define HARDWARE_H

/* base structure for all hardware components */
typedef struct {
	int id;
	char *name;
	int debug;
} Hardware;

void hardware_init(Hardware *hw, int id, const char *name);
void hardware_log(Hardware *hw, const char *message);
char *hexLog(unsigned int number, int length);

#endif // HARDWARE_H
