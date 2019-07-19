#include <stdint.h>

#include "machine.h"
#include "debug.h"

const auto OSC1_SPEED	= 4000000;
const auto OSC3_SPEED	= 32768;
const auto MS_SPEED		= 1000;
const auto CPU_DIVIDER	= 4;

static const uint8_t bios[0x2000] = {
	#include "bios.h"
};

__attribute__ ((visibility ("default"))) extern "C"
MachineState* const get_machine() {
	static MachineState state;
	return &state;
}

__attribute__ ((visibility ("default"))) extern "C"
void cpu_reset(MachineState& cpu) {
	cpu.reg.pc = cpu_read16(cpu, 0x0000);
	cpu.reg.sc = 0xC0;
	cpu.reg.ep = 0xFF;
	cpu.reg.xp = 0x00;
	cpu.reg.yp = 0x00;
	cpu.reg.nb = 0x01;

	cpu.sleeping = false;
	cpu.halted = false;

	irq_reset(cpu);
	lcd_reset(cpu);
}

/**
 * S1C88 Memory access helper functions
 **/

uint16_t cpu_read16(MachineState& cpu, uint32_t address) {
	uint16_t lo = cpu_read8(cpu, address);
	address = ((address + 1) & 0xFFFF) | (address & 0xFF0000);
	return (cpu_read8(cpu, address) << 8) | lo;
}

void cpu_write16(MachineState& cpu, uint16_t data, uint32_t address) {
	cpu_write8(cpu, (uint8_t) data, address);
	address = ((address + 1) & 0xFFFF) | (address & 0xFF0000);
	cpu_write8(cpu, data >> 8, address);
}

uint8_t cpu_imm8(MachineState& cpu) {
	uint16_t address = cpu.reg.pc++;

	if (address & 0x8000) {
		address = (cpu.reg.cb << 15) | (address & 0x7FFF);
	}
	return cpu_read8(cpu, address);
}

uint16_t cpu_imm16(MachineState& cpu) {
	uint8_t lo = cpu_imm8(cpu);
	return (cpu_imm8(cpu) << 8) | lo;
}

void cpu_push8(MachineState& cpu, uint8_t t) {
	cpu_write8(cpu, t, --cpu.reg.sp);
}

uint8_t cpu_pop8(MachineState& cpu) {
	return cpu_read8(cpu, cpu.reg.sp++);
}

void cpu_push16(MachineState& cpu, uint16_t t) {
	cpu_push8(cpu, t >> 8);
	cpu_push8(cpu, (uint8_t)t);
}

uint16_t cpu_pop16(MachineState& cpu) {
	uint16_t t = cpu_pop8(cpu);
	return (cpu_pop8(cpu) << 8) | t;
}

__attribute__ ((visibility ("default")))
void cpu_step(MachineState& cpu) {
	irq_fire(cpu);
	
	if (!cpu.sleeping && !cpu.halted) {
		inst_advance(cpu);
	} else {
		// TODO: THIS SHOULD BE SMARTER
		cpu.clocks--;
	}
}

__attribute__ ((visibility ("default")))
bool cpu_advance(MachineState& cpu, int ticks) {
	cpu.clocks += OSC1_SPEED / CPU_DIVIDER / MS_SPEED * ticks;

	while (cpu.clocks > 0) {
		cpu_step(cpu);
	}

	return true;
}

uint8_t cpu_read_reg(MachineState& cpu, uint32_t address) {
	switch (address) {
	case 0x2020: case 0x2021: case 0x2022:
	case 0x2023: case 0x2024: case 0x2025: case 0x2026:
	case 0x2027: case 0x2028: case 0x2029: case 0x202A:
		return irq_read_reg(cpu, address);
	case 0x20FE: case 0x20FF:
		return lcd_read_reg(cpu, address);
	default:
		//dprintf("Unhandled register read %x", address);
		return cpu.bus_cap;
	}
}

void cpu_write_reg(MachineState& cpu, uint8_t data, uint32_t address) {
	switch (address) {
	case 0x2020: case 0x2021: case 0x2022:
	case 0x2023: case 0x2024: case 0x2025: case 0x2026:
	case 0x2027: case 0x2028: case 0x2029: case 0x202A:
		irq_write_reg(cpu, data, address);
		break ;
	case 0x20FE: case 0x20FF:
		lcd_write_reg(cpu, data, address);
		break ;
	default:
		//dprintf("Unhandled register write %x: %x", address, data);
		break ;
	}
}

__attribute__ ((visibility ("default")))
uint8_t cpu_read8(MachineState& cpu, uint32_t address) {
	switch (address) {
		case 0x0000 ... 0x0FFF:
			return cpu.bus_cap = bios[address];
		case 0x1000 ... 0x1FFF:
			return cpu.bus_cap = cpu.ram[address & 0xFFF];
		case 0x2000 ... 0x20FF:
			return cpu.bus_cap = cpu_read_reg(cpu, address);
		default:
			return cpu.bus_cap = cpu_read_cart(cpu, address);		
	}
}

__attribute__ ((visibility ("default")))
void cpu_write8(MachineState& cpu, uint8_t data, uint32_t address) {
	cpu.bus_cap = data;
	
	switch (address) {
		case 0x0000 ... 0x0FFF:
			break ;
		case 0x1000 ... 0x1FFF:
			cpu.ram[address & 0xFFF] = data;
			break ;
		case 0x2000 ... 0x20FF:
			cpu_write_reg(cpu, data, address);
			break ;
		default:
			cpu_write_cart(cpu, data, address);
			break ;	
	}
}
