#include <stdint.h>

#include "system.h"

void cpu_int(ProcessorState& cpu, InterruptVector irq, int priority) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);
	cpu_push8(cpu, cpu.reg.sc);

	cpu.reg.pc = cpu_read16(cpu, 2 * (int) irq);
	cpu.reg.flag.i = priority;
}

/**
 * S1C88 Memory access functions
 **/

uint16_t cpu_read16(ProcessorState& cpu, uint32_t address) {
	uint16_t lo = cpu_read8(cpu, address);
	address = ((address + 1) & 0xFFFF) | (address & 0xFF0000);
	return (cpu_read8(cpu, address) << 8) | lo;
}

void cpu_write16(ProcessorState& cpu, uint16_t data, uint32_t address) {
	cpu_write8(cpu, (uint8_t) data, address);
	address = ((address + 1) & 0xFFFF) | (address & 0xFF0000);
	cpu_write8(cpu, data >> 8, address);
}

uint8_t cpu_imm8(ProcessorState& cpu) {
	uint32_t address = cpu.reg.pc++;

	if (address & 0x8000) {
		address = (address & 0x7FFF) | (cpu.reg.cb << 15);
	}

	return cpu_read8(cpu, address);
}

uint16_t cpu_imm16(ProcessorState& cpu) {
	uint8_t lo = cpu_imm8(cpu);
	return (cpu_imm8(cpu) << 8) | lo;
}

void cpu_push8(ProcessorState& cpu, uint8_t t) {
	cpu.reg.sp--;
	cpu_write8(cpu, t, cpu.reg.sp);
}

uint8_t cpu_pop8(ProcessorState& cpu) {
	return cpu_read8(cpu, cpu.reg.sp++);
}

void cpu_push16(ProcessorState& cpu, uint16_t t) {
	cpu_push8(cpu, (uint8_t)t);
	cpu_push8(cpu, t >> 8);
}

uint16_t cpu_pop16(ProcessorState& cpu) {
	uint16_t t = cpu_pop8(cpu) << 8;
	return cpu_pop8(cpu) | t;
}
