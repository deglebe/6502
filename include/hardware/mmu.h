/* include/hardware/mmu.h
 * mmu translates cpu vis address to phys ram ident map for now
 *
 * by: thomas bruce
 * date: 04/11/2026
 */

#ifndef MMU_H
#define MMU_H

#include <stdint.h>

#include "hardware/cpu.h"
#include "hardware/memory.h"

/* cpu and ram talk only through the mmu: logical mar lives here; ram keeps
 * its own mar/mdr for the physical bus cycle */
typedef struct {
	Hardware hardware;
	Cpu *cpu;
	Memory *ram;
	/* logical memory address register (16-bit); default 0x0000 */
	uint16_t mar;
} Mmu;

int mmu_init(Mmu *mmu, Cpu *cpu, Memory *ram);

uint16_t mmu_get_mar(const Mmu *mmu);
/* sets full logical mar (high and low bytes replaced) */
void mmu_set_mar(Mmu *mmu, uint16_t virt_addr);

/* update only one byte of mar; other byte unchanged (bitmasks) */
void mmu_set_mar_low_byte(Mmu *mmu, uint8_t lob);
void mmu_set_mar_high_byte(Mmu *mmu, uint8_t hob);

/* read low byte from virt_addr, high from virt_addr+1 (le), combine into mar */
void mmu_mar_load_from_le(Mmu *mmu, uint16_t le_addr);

/* virtual -> physical; identity map until paging exists */
uint16_t mmu_translate(const Mmu *mmu, uint16_t virt_addr);

/* one bus cycle at logical mar: translate, latch ram mar, read/write mdr */
uint8_t mmu_read(Mmu *mmu);
void mmu_write(Mmu *mmu, uint8_t value);

/* thin wrappers used by code that already set logical mar */
void mmu_bus_read(Mmu *mmu);
void mmu_bus_write(Mmu *mmu);

#endif // MMU_H
