#ifndef SYSTEM_H
#define SYSTEM_H

#include "hardware/hardware.h"
#include "hardware/cpu.h"

typedef struct {
    Hardware hardware;
    Cpu* _CPU;
    int running;
} System;

void system_init(System* sys);
int system_start(System* sys);
int system_stop(System* sys);
void system_cleanup(System* sys);

#endif // SYSTEM_H

