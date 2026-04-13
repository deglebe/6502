/* src/hardware/mmu.c
 *
 * by: thomas bruce
 * date: 04/11/2026
 */

#include <stdio.h>

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
	mmu->read_ready_cycle = 0;
	mmu->write_ready_cycle = 0;
	mmu->read_pending = 0;
	mmu->write_pending = 0;
	return 0;
}

int mmu_initialize_memory(Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL) {
		return -1;
	}
	return memory_init(mmu->ram);
}

int mmu_load_startup_program(Mmu *mmu) {
	/* static program:
	 * 0000:A9 0001:0D 0002:A9 0003:1D 0004:A9 0005:2D 0006:A9 0007:3F 0008:A9 0009:FF 000A:00
	 */
	static const uint8_t program[] = {
		0xA9, 0x0D, 0xA9, 0x1D, 0xA9, 0x2D, 0xA9, 0x3F, 0xA9, 0xFF, 0x00
	};

	if (mmu == NULL || mmu->ram == NULL) {
		return -1;
	}

	for (uint16_t i = 0x0000; i < (uint16_t)(sizeof(program)); i++) {
		mmu_write_immediate(mmu, i, program[i]);
	}
	hardware_log(&mmu->hardware, "Initialized Memory");
	return 0;
}

void mmu_memory_display(Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	memory_display(mmu->ram);
}

void mmu_memory_dump(Mmu *mmu, uint16_t start, uint16_t length) {
	char addr_str[8];
	char value_str[8];
	char line[64];

	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	hardware_log(&mmu->hardware, "Memory Dump: Debug");
	hardware_log(&mmu->hardware, "--------------------------------------");

	for (uint32_t off = 0; off < (uint32_t)length; off++) {
		uint16_t addr = (uint16_t)(start + (uint16_t)off);
		uint8_t value = memory_peek(mmu->ram, addr);

		if (hexLog(addr_str, sizeof(addr_str), addr, 4) != 0
			|| hexLog(value_str, sizeof(value_str), value, 2) != 0) {
			hardware_log(&mmu->hardware, "Addr ERR: | ERR");
			continue;
		}

		snprintf(line, sizeof(line), "Addr %s: | %s", addr_str, value_str);
		hardware_log(&mmu->hardware, line);
	}

	hardware_log(&mmu->hardware, "--------------------------------------");
	hardware_log(&mmu->hardware, "Memory Dump: Complete");
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
	/* keep high byte (FF00), replace low byte with lob */
	mmu->mar = (mmu->mar & 0xFF00u) | (uint16_t)(lob & 0xFFu);
}

void mmu_set_mar_high_byte(Mmu *mmu, uint8_t hob) {
	if (mmu == NULL) {
		return;
	}
	/* keep low byte (00FF), shift hob into high byte position */
	mmu->mar = (mmu->mar & 0x00FFu) | ((uint16_t)(hob & 0xFFu) << 8);
}

void mmu_mar_load_from_le(Mmu *mmu, uint16_t le_addr) {
	uint8_t lo;
	uint8_t hi;

	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}

	/* loading mar bytes does not trigger a memory bus read cycle; this is
	 * just decoding two already-present bytes in little-endian order */
	lo = memory_peek(mmu->ram, mmu_translate(mmu, le_addr));
	hi = memory_peek(mmu->ram, mmu_translate(mmu, (uint16_t)(le_addr + 1u)));

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
	/* first call schedules read; caller should poll readiness */
	if (!mmu->read_pending) {
		mmu_sync_physical_mar(mmu);
		memory_request_read(mmu->ram);
		mmu->read_pending = 1;
		mmu->read_ready_cycle = memory_get_cycle(mmu->ram) + 1;
		return 0;
	}

	if (mmu_is_read_ready(mmu)) {
		mmu->read_pending = 0;
		return memory_get_mdr(mmu->ram);
	}
	return 0;
}

void mmu_write(Mmu *mmu, uint8_t value) {
	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	mmu_sync_physical_mar(mmu);
	memory_set_mdr(mmu->ram, value);
	memory_request_write(mmu->ram);
	mmu->write_pending = 1;
	mmu->write_ready_cycle = memory_get_cycle(mmu->ram) + 1;
}

void mmu_write_immediate(Mmu *mmu, uint16_t addr, uint8_t value) {
	if (mmu == NULL) {
		return;
	}
	mmu_set_mar(mmu, addr);
	mmu_write(mmu, value);
	/* bootstrapping helper: force one pulse so write commits on-cycle */
	memory_pulse(mmu->ram);
	mmu->write_pending = 0;
}

int mmu_is_read_ready(const Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL || !mmu->read_pending) {
		return 0;
	}
	return memory_get_cycle(mmu->ram) >= mmu->read_ready_cycle && !memory_is_busy(mmu->ram);
}

int mmu_is_write_ready(const Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL || !mmu->write_pending) {
		return 0;
	}
	return memory_get_cycle(mmu->ram) >= mmu->write_ready_cycle && !memory_is_busy(mmu->ram);
}

uint64_t mmu_get_read_ready_cycle(const Mmu *mmu) {
	if (mmu == NULL) {
		return 0;
	}
	return mmu->read_ready_cycle;
}

uint64_t mmu_get_write_ready_cycle(const Mmu *mmu) {
	if (mmu == NULL) {
		return 0;
	}
	return mmu->write_ready_cycle;
}

void mmu_bus_read(Mmu *mmu) {
	(void)mmu_read(mmu);
}

void mmu_bus_write(Mmu *mmu) {
	if (mmu == NULL || mmu->ram == NULL) {
		return;
	}
	mmu_write(mmu, memory_get_mdr(mmu->ram));
}
