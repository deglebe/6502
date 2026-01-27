/* include/system.h
 *
 * by: thomas bruce
 * date: 27/01/2026
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include "hardware/hardware.h"
#include "hardware/cpu.h"
#include "hardware/memory.h"

typedef struct {
	Hardware hardware;
	Cpu *_CPU;
	Memory *_Memory;
	int running;
} System;

void system_init(System *sys);
int system_start(System *sys);
int system_stop(System *sys);
void system_cleanup(System *sys);

#endif // SYSTEM_H
