/* include/hardware/imp/clock_listener.h
 *
 * by: thomas bruce
 * date: 30/01/2026
 */

#ifndef CLOCK_LISTENER_H
#define CLOCK_LISTENER_H

/* function pointer for clock pulse notification
 * can't really do the same extension thing as in typescript
 * takes void pointer context
 */
typedef void (*ClockPulseFunc)(void *context);

/* maybe i can passably just pretend c has interfaces xD */
typedef struct {
	ClockPulseFunc pulse;
	void *context;
} ClockListener;

#endif // CLOCK_LISTENER_H
