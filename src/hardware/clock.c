/* src/hardware/clock.c
 * clock operations
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include "hardware/clock.h"

void clock_init(Clock *clock) {
	hardware_init(&clock->hardware, 0, "clock");
	clock->listener_count = 0;
}

void clock_register_listener(Clock *clock, ClockListener listener) {
	if (clock->listener_count < MAX_LISTENERS) {
		clock->listeners[clock->listener_count] = listener;
		clock->listener_count++;
	}
}

void clock_pulse(Clock *clock) {
	/* notify all registered listeners on clock pulse */
	for (int i = 0; i < clock->listener_count; i++) {
		if (clock->listeners[i].pulse) {
			clock->listeners[i].pulse(clock->listeners[i].context);
		}
	}
}

void clock_run(Clock *clock, int interval_ms) {
	while (1) {
		hardware_log(&clock->hardware, "clock pulse initialized");
		clock_pulse(clock);
		/* c99 doesn't really like usleep so setting the posix standard
		 * above and using nanosleep is kind of the cleanest solution
		 *
		 * i could use a busy loop instead but i don't think that will
		 * scale well and, let's face it, this code wastes enough clock
		 * cycles as it is
		 */
		struct timespec ts;
		ts.tv_sec = interval_ms / 1000;
		ts.tv_nsec = (interval_ms % 1000) * 1000000;
		nanosleep(&ts, NULL);
	}
}
