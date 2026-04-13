/* include/hardware/mmu.h
 * memory management unit — translates cpu-visible addresses to physical
 * ram. identity map for now; mar/mdr traffic goes through the ram chip.
 *
 * by: thomas bruce
 * date: 04/04/2026
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
