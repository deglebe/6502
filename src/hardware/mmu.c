/* src/hardware/mmu.c
 *
 * by: thomas bruce
 * date: 04/04/2026
 */

#include "hardware/mmu.h"

int mmu_init(Mmu *mmu, Memory *ram) {
	if (mmu == NULL || ram == NULL) {
		return -1;
	}
	if (hardware_init(&mmu->hardware, 0, "mmu") != 0) {
		return -1;
	}
	mmu->ram = ram;
	return 0;
}

uint16_t mmu_translate(const Mmu *mmu, uint16_t virt_addr) {
	(void)mmu;
	return virt_addr;
}

void mmu_set_mar(Mmu *mmu, uint16_t virt_addr) {
	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	memory_set_mar(mmu->ram, mmu_translate(mmu, virt_addr));
}

void mmu_bus_read(Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	memory_bus_read(mmu->ram);
}

void mmu_bus_write(Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	memory_bus_write(mmu->ram);
}
