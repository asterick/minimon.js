#include <stdint.h>

#include "machine.h"
#include "irq.h"

struct IRQVector {
	uint8_t* const prio_reg;
	const uint8_t  prio_shift;

};

void irq_reset(MachineState& cpu) {
	// TODO	
}

void cpu_interrupt(MachineState& cpu, InterruptVector irq, int level) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);
	cpu_push8(cpu, cpu.reg.sc);

	cpu.reg.pc = cpu_read16(cpu, 2 * (int) irq);
	cpu.reg.flag.i = level;
}

void irq_trigger(InterruptVector irq) {
	// TODO
}

uint8_t irq_read_reg(MachineState& cpu, uint32_t address) {
	switch (address) {
		case 0x2020: return cpu.irq.priority;
		case 0x2021: return cpu.irq.priority >> 8;
		case 0x2022: return cpu.irq.priority >> 16;
		case 0x2023: return cpu.irq.enable;
		case 0x2024: return cpu.irq.enable >> 8;
		case 0x2025: return cpu.irq.enable >> 16;
		case 0x2026: return cpu.irq.enable >> 24;
		case 0x2027: return cpu.irq.active;
		case 0x2028: return cpu.irq.active >> 8;
		case 0x2029: return cpu.irq.active >> 16;
		case 0x202A: return cpu.irq.active >> 24;
			return 0xCD;
	}
	return 0xFF;
}

void irq_write_reg(MachineState& cpu, uint8_t data, uint32_t address) {
	switch (address) {
		case 0x2020:
		case 0x2021:
		case 0x2022:
		case 0x2023:
		case 0x2024:
		case 0x2025:
		case 0x2026:
		case 0x2027:
		case 0x2028:
		case 0x2029:
		case 0x202A:
			break ;
	}
}
