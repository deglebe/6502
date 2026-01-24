/* src/hardware/cpu.c
 * cpu-specific operations
 *
 * by: thomas bruce
 * date: 24/01/2026
 */

#include "hardware/cpu.h"

void cpu_init(Cpu *cpu) {
	hardware_init(&cpu->hardware, 0, "cpu");
}

void cpu_log(Cpu *cpu, const char *message) {
	hardware_log(&cpu->hardware, message);
}
