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
	sys->_Mmu = NULL;
	sys->_Clock = NULL;
	sys->running = 0;

	if (hardware_init(&sys->hardware, 0, "system") != 0) {
		fprintf(stderr, "6502: hardware_init failed (system)\n");
		return;
	}

	sys->_CPU = (Cpu *) malloc(sizeof(Cpu));
	if (sys->_CPU == NULL) {
		fprintf(stderr, "6502: malloc failed (cpu)\n");
		free(sys->hardware.name);
		sys->hardware.name = NULL;
		return;
	}
	if (cpu_init(sys->_CPU) != 0) {
		fprintf(stderr, "6502: hardware_init failed (cpu)\n");
		free(sys->_CPU);
		sys->_CPU = NULL;
		free(sys->hardware.name);
		sys->hardware.name = NULL;
		return;
	}

	sys->_Memory = (Memory *) malloc(sizeof(Memory));
	if (sys->_Memory == NULL) {
		fprintf(stderr, "6502: malloc failed (memory)\n");
		free(sys->_CPU->hardware.name);
		free(sys->_CPU);
		sys->_CPU = NULL;
		free(sys->hardware.name);
		sys->hardware.name = NULL;
		return;
	}

	sys->_Mmu = (Mmu *) malloc(sizeof(Mmu));
	if (sys->_Mmu == NULL) {
		fprintf(stderr, "6502: malloc failed (mmu)\n");
		free(sys->_Memory);
		sys->_Memory = NULL;
		free(sys->_CPU->hardware.name);
		free(sys->_CPU);
		sys->_CPU = NULL;
		free(sys->hardware.name);
		sys->hardware.name = NULL;
		return;
	}

	sys->_Clock = (Clock *) malloc(sizeof(Clock));
	if (sys->_Clock == NULL) {
		fprintf(stderr, "6502: malloc failed (clock)\n");
		free(sys->_Mmu);
		sys->_Mmu = NULL;
		/* memory_init not run yet, as only the struct was allocated */
		free(sys->_Memory);
		sys->_Memory = NULL;
		free(sys->_CPU->hardware.name);
		free(sys->_CPU);
		sys->_CPU = NULL;
		free(sys->hardware.name);
		sys->hardware.name = NULL;
		return;
	}
	if (clock_init(sys->_Clock) != 0) {
		fprintf(stderr, "6502: hardware_init failed (clock)\n");
		if (sys->_Clock->hardware.name != NULL) {
			free(sys->_Clock->hardware.name);
		}
		free(sys->_Clock);
		sys->_Clock = NULL;
		free(sys->_Mmu);
		sys->_Mmu = NULL;
		free(sys->_Memory);
		sys->_Memory = NULL;
		free(sys->_CPU->hardware.name);
		free(sys->_CPU);
		sys->_CPU = NULL;
		free(sys->hardware.name);
		sys->hardware.name = NULL;
		return;
	}

	system_start(sys);
}

int system_start(System *sys) {
	if (sys->_CPU == NULL || sys->_Memory == NULL || sys->_Mmu == NULL
		|| sys->_Clock == NULL) {
		fprintf(stderr, "6502: system_start called with incomplete allocation\n");
		return 0;
	}

	/* toggle cpu debug output */
	sys->_CPU->hardware.debug = 1;
	hardware_log(&sys->hardware, "created");
	cpu_log(sys->_CPU, "created");

	if (mmu_init(sys->_Mmu, sys->_CPU, sys->_Memory) != 0) {
		fprintf(stderr, "6502: mmu_init failed\n");
		return 0;
	}
	/* initialize memory through mmu authority boundary */
	if (mmu_initialize_memory(sys->_Mmu) != 0) {
		fprintf(stderr, "6502: hardware_init failed (memory)\n");
		return 0;
	}

	mmu_memory_display(sys->_Mmu);

	/* exercise mmu logical mar + read/write + le load + low/high byte masks */
	mmu_write_immediate(sys->_Mmu, 0x0200, 0xA9);
	mmu_write_immediate(sys->_Mmu, 0x0201, 0xEA);
	/* store le pointer 0x34, 0x12 at 0x0010 -> mar should become 0x1234 */
	mmu_write_immediate(sys->_Mmu, 0x0010, 0x34);
	mmu_write_immediate(sys->_Mmu, 0x0011, 0x12);
	mmu_mar_load_from_le(sys->_Mmu, 0x0010);
	mmu_set_mar_low_byte(sys->_Mmu, 0x15);
	mmu_set_mar_high_byte(sys->_Mmu, 0xAB);
	mmu_memory_dump(sys->_Mmu, 0x0000, 0x20);
	mmu_memory_dump(sys->_Mmu, 0x0200, 0x10);

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
	if (sys->_Mmu != NULL) {
		if (sys->_Mmu->hardware.name != NULL) {
			free(sys->_Mmu->hardware.name);
		}
		free(sys->_Mmu);
		sys->_Mmu = NULL;
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
