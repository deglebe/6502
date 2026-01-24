/* src/main.c
 * main entrypoint for 6502 emulator
 *
 * by: thomas bruce
 * date: 24/01/2026
 */

#include "system.h"

int main(void) {
	System system;
	system_init(&system);
	system_cleanup(&system);
	return 0;
}
