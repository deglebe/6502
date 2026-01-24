/* src/system.c
 * system-wide functions
 *
 * by: thomas bruce
 * date: 24/01/2026
 */

#include <stdlib.h>

#include "system.h"

void system_init(System *sys) {
	hardware_init(&sys->hardware, 0, "system");
	sys->_CPU = (Cpu *) malloc(sizeof(Cpu));
	cpu_init(sys->_CPU);
	sys->running = 0;

	system_start(sys);
}

int system_start(System *sys) {
	sys->_CPU->hardware.debug = 0;
	hardware_log(&sys->hardware, "created");
	cpu_log(sys->_CPU, "created");
	return 1;
}

int system_stop(System *sys) {
	return 0;
}

void system_cleanup(System *sys) {
	if (sys->_CPU) {
		free(sys->_CPU->hardware.name);
		free(sys->_CPU);
	}
	free(sys->hardware.name);
}
