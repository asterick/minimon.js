#include <stdint.h>

#include "machine.h"
#include "irq.h"

void cpu_interrupt(ProcessorState& cpu, InterruptVector irq, int level) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);
	cpu_push8(cpu, cpu.reg.sc);

	cpu.reg.pc = cpu_read16(cpu, 2 * (int) irq);
	cpu.reg.flag.i = level;
}

void irq_trigger(InterruptVector irq) {
	// TODO
}

uint8_t irq_read_reg(ProcessorState& cpu, uint32_t address) {
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
			return 0xCD;
	}
	return 0xFF;
}

void irq_write_reg(ProcessorState& cpu, uint8_t data, uint32_t address) {
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
