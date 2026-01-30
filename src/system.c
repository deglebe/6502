/* src/system.c
 * system-wide functions
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#include <stdlib.h>

#include "system.h"

void system_init(System *sys) {
	hardware_init(&sys->hardware, 0, "system");
	/* allocate and init cpu. mem alloc happens here but init is
	 * deferred to system_start */
	sys->_CPU = (Cpu *) malloc(sizeof(Cpu));
	cpu_init(sys->_CPU);
	sys->_Memory = (Memory *) malloc(sizeof(Memory));
	sys->_Clock = (Clock *) malloc(sizeof(Clock));
	clock_init(sys->_Clock);
	sys->running = 0;

	system_start(sys);
}

int system_start(System *sys) {
	/* toggle cpu debug output */
	sys->_CPU->hardware.debug = 1;
	hardware_log(&sys->hardware, "created");
	cpu_log(sys->_CPU, "created");

	/* init mem and display contents. ought to show properly zeroed memory
	 * if not, ¯\_(ツ)_/¯ */
	if (sys->_Memory) {
		memory_init(sys->_Memory);
		memory_display(sys->_Memory);
	}

	/* register cpu and mem as clock listeners */
	if (sys->_Clock && sys->_CPU && sys->_Memory) {
		ClockListener cpu_listener = { cpu_pulse, sys->_CPU };
		ClockListener memory_listener = { memory_pulse, sys->_Memory };
		clock_register_listener(sys->_Clock, cpu_listener);
		clock_register_listener(sys->_Clock, memory_listener);

		/* start the clock running. pulse on CLOCK_INTERVAL */
		clock_run(sys->_Clock, CLOCK_INTERVAL);
	}

	return 1;
}

int system_stop(System *sys) {
	return 0;
}

void system_cleanup(System *sys) {
	/* free all allocated resources */
	if (sys->_CPU) {
		free(sys->_CPU->hardware.name);
		free(sys->_CPU);
	}
	if (sys->_Memory) {
		free(sys->_Memory->hardware.name);
		free(sys->_Memory);
	}
	if (sys->_Clock) {
		free(sys->_Clock->hardware.name);
		free(sys->_Clock);
	}
	free(sys->hardware.name);
}
