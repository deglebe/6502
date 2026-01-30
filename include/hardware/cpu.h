/* include/hardware/cpu.h
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#ifndef CPU_H
#define CPU_H

#include "hardware/hardware.h"

typedef struct {
	Hardware hardware;
	int cpuClockCount;
} Cpu;

void cpu_init(Cpu *cpu);
void cpu_log(Cpu *cpu, const char *message);
void cpu_pulse(void *context);

#endif // CPU_H
