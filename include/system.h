/* include/system.h
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include "hardware/hardware.h"
#include "hardware/cpu.h"
#include "hardware/memory.h"
#include "hardware/clock.h"
#include "hardware/imp/clock_listener.h"

/* clock interval in milliseconds */
#define CLOCK_INTERVAL 1000

typedef struct {
	Hardware hardware;
	Cpu *_CPU;
	Memory *_Memory;
	Clock *_Clock;
	int running;
} System;

void system_init(System *sys);
int system_start(System *sys);
int system_stop(System *sys);
void system_cleanup(System *sys);

#endif // SYSTEM_H
