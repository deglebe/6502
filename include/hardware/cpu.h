#ifndef CPU_H
#define CPU_H

#include "hardware/hardware.h"

typedef struct {
    Hardware hardware;
} Cpu;

void cpu_init(Cpu* cpu);
void cpu_log(Cpu* cpu, const char* message);

#endif // CPU_H

