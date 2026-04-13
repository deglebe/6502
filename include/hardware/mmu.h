/* include/hardware/mmu.h
 *
 * by: thomas bruce
 * date: 04/11/2026
 */

#ifndef MMU_H
#define MMU_H

#include <stdint.h>

#include "hardware/hardware.h"
#include "hardware/memory.h"

typedef struct {
	Hardware hardware;
	Memory *ram;
} Mmu;

int mmu_init(Mmu *mmu, Memory *ram);
/* virtual -> physical; no relocation yet, so virt == phys */
uint16_t mmu_translate(const Mmu *mmu, uint16_t virt_addr);
void mmu_set_mar(Mmu *mmu, uint16_t virt_addr);
void mmu_bus_read(Mmu *mmu);
void mmu_bus_write(Mmu *mmu);

#endif // MMU_H
