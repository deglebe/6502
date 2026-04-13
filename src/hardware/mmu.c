/* src/hardware/mmu.c
 *
 * by: thomas bruce
 * date: 04/11/2026
 */

#include "hardware/cpu.h"
#include "hardware/mmu.h"

int mmu_init(Mmu *mmu, Cpu *cpu, Memory *ram) {
	if (mmu == NULL || ram == NULL) {
		return -1;
	}
	if (hardware_init(&mmu->hardware, 0, "mmu") != 0) {
		return -1;
	}
	mmu->cpu = cpu;
	mmu->ram = ram;
	mmu->mar = 0x0000;
	return 0;
}

uint16_t mmu_get_mar(const Mmu *mmu) {
	if (mmu == NULL) {
		return 0;
	}
	return mmu->mar;
}

void mmu_set_mar(Mmu *mmu, uint16_t virt_addr) {
	if (mmu == NULL) {
		return;
	}
	mmu->mar = virt_addr;
}

void mmu_set_mar_low_byte(Mmu *mmu, uint8_t lob) {
	if (mmu == NULL) {
		return;
	}
	mmu->mar = (mmu->mar & 0xFF00u) | (uint16_t)(lob & 0xFFu);
}

void mmu_set_mar_high_byte(Mmu *mmu, uint8_t hob) {
	if (mmu == NULL) {
		return;
	}
	mmu->mar = (mmu->mar & 0x00FFu) | ((uint16_t)(hob & 0xFFu) << 8);
}

void mmu_mar_load_from_le(Mmu *mmu, uint16_t le_addr) {
	uint8_t lo;
	uint8_t hi;

	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}

	memory_set_mar(mmu->ram, mmu_translate(mmu, le_addr));
	memory_bus_read(mmu->ram);
	lo = memory_get_mdr(mmu->ram);

	memory_set_mar(mmu->ram, mmu_translate(mmu, (uint16_t)(le_addr + 1u)));
	memory_bus_read(mmu->ram);
	hi = memory_get_mdr(mmu->ram);

	mmu->mar = (uint16_t)lo | ((uint16_t)hi << 8);
}

uint16_t mmu_translate(const Mmu *mmu, uint16_t virt_addr) {
	(void)mmu;
	return virt_addr;
}

static void mmu_sync_physical_mar(Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	memory_set_mar(mmu->ram, mmu_translate(mmu, mmu->mar));
}

uint8_t mmu_read(Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL) {
		return 0;
	}
	mmu_sync_physical_mar(mmu);
	memory_bus_read(mmu->ram);
	return memory_get_mdr(mmu->ram);
}

void mmu_write(Mmu *mmu, uint8_t value) {
	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	mmu_sync_physical_mar(mmu);
	memory_set_mdr(mmu->ram, value);
	memory_bus_write(mmu->ram);
}

void mmu_bus_read(Mmu *mmu) {
	(void)mmu_read(mmu);
}

void mmu_bus_write(Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	mmu_sync_physical_mar(mmu);
	memory_bus_write(mmu->ram);
}
