/* src/hardware/cpu.c
 * cpu-specific operations
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#include <stdio.h>
#include "hardware/cpu.h"

void cpu_init(Cpu *cpu) {
	hardware_init(&cpu->hardware, 0, "cpu");
	cpu->cpuClockCount = 0;
}

void cpu_log(Cpu *cpu, const char *message) {
	hardware_log(&cpu->hardware, message);
}

void cpu_pulse(void *context) {
	Cpu *cpu = (Cpu *) context;
	cpu->cpuClockCount++;

	char message[128];
	snprintf(message,
		sizeof(message),
		"received clock pulse - CPU Clock Count: %d",
		cpu->cpuClockCount);
	hardware_log(&cpu->hardware, message);
}
