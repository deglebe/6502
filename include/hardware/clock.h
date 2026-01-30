/* include/hardware/clock.h
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#ifndef CLOCK_H
#define CLOCK_H

#include "hardware/hardware.h"
#include "hardware/imp/clock_listener.h"

#define MAX_LISTENERS 16

typedef struct {
	Hardware hardware;
	ClockListener listeners[MAX_LISTENERS];
	int listener_count;
} Clock;

void clock_init(Clock *clock);
void clock_register_listener(Clock *clock, ClockListener listener);
void clock_pulse(Clock *clock);
void clock_run(Clock *clock, int interval_ms);

#endif // CLOCK_H
