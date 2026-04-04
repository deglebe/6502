/* src/system.c
 * system-wide functions
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#include <stdio.h>
#include <stdlib.h>

#include "system.h"

void system_init(System *sys) {
	sys->_CPU = NULL;
	sys->_Memory = NULL;
	sys->_Clock = NULL;
	sys->running = 0;

	hardware_init(&sys->hardware, 0, "system");

	sys->_CPU = (Cpu *) malloc(sizeof(Cpu));
	if (sys->_CPU == NULL) {
		fprintf(stderr, "6502: malloc failed (cpu)\n");
		return;
	}
	cpu_init(sys->_CPU);

	sys->_Memory = (Memory *) malloc(sizeof(Memory));
	if (sys->_Memory == NULL) {
		fprintf(stderr, "6502: malloc failed (memory)\n");
		free(sys->_CPU->hardware.name);
		free(sys->_CPU);
		sys->_CPU = NULL;
		return;
	}

	sys->_Clock = (Clock *) malloc(sizeof(Clock));
	if (sys->_Clock == NULL) {
		fprintf(stderr, "6502: malloc failed (clock)\n");
		free(sys->_Memory->hardware.name);
		free(sys->_Memory);
		sys->_Memory = NULL;
		free(sys->_CPU->hardware.name);
		free(sys->_CPU);
		sys->_CPU = NULL;
		return;
	}
	clock_init(sys->_Clock);

	system_start(sys);
}

int system_start(System *sys) {
	if (sys->_CPU == NULL || sys->_Memory == NULL || sys->_Clock == NULL) {
		fprintf(stderr, "6502: system_start called with incomplete allocation\n");
		return 0;
	}

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
	if (sys->_CPU != NULL) {
		if (sys->_CPU->hardware.name != NULL) {
			free(sys->_CPU->hardware.name);
		}
		free(sys->_CPU);
		sys->_CPU = NULL;
	}
	if (sys->_Memory != NULL) {
		if (sys->_Memory->hardware.name != NULL) {
			free(sys->_Memory->hardware.name);
		}
		free(sys->_Memory);
		sys->_Memory = NULL;
	}
	if (sys->_Clock != NULL) {
		if (sys->_Clock->hardware.name != NULL) {
			free(sys->_Clock->hardware.name);
		}
		free(sys->_Clock);
		sys->_Clock = NULL;
	}
	if (sys->hardware.name != NULL) {
		free(sys->hardware.name);
		sys->hardware.name = NULL;
	}
}
