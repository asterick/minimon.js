#include <stdint.h>

#include "machine.h"
#include "debug.h"

const auto OSC1_SPEED	= 4000000;
const auto OSC3_SPEED	= 32768;
const auto TICK_SPEED	= 1000;
const auto CPU_SPEED	= 1000000;

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
	cpu.osc3_overflow = 0;

	irq_reset(cpu);
	lcd_reset(cpu);
	rtc_reset(cpu);
	tim256_reset(cpu);
}

void cpu_clock(MachineState& cpu, int cycles) {
	int osc1 = cycles * OSC1_SPEED / CPU_SPEED;	
	int osc3 = 0;

	cpu.osc3_overflow += osc1 * OSC3_SPEED;

	if (cpu.osc3_overflow >= OSC1_SPEED) {
		// Assume we are not going to get more than a couple ticks out of this thing
		do {
			cpu.osc3_overflow -= OSC1_SPEED;
			osc3++;
		} while (cpu.osc3_overflow >= OSC1_SPEED);

		// These are the devices that only advance with OSC3
		tim256_clock(cpu, osc3);
		rtc_clock(cpu, osc3);
 	} else {
 		osc3 = 0;
 	}

 	// OSC1 = 4mhz oscillator, OSC3 = 32khz oscillator
	cpu.clocks -= osc1;
}

__attribute__ ((visibility ("default")))
void cpu_step(MachineState& cpu) {
	irq_fire(cpu);
	
	if (!cpu.sleeping && !cpu.halted) {
		cpu_clock(cpu, inst_advance(cpu));
	} else {
		cpu_clock(cpu, 1); // This is lazy
	}
}

__attribute__ ((visibility ("default")))
bool cpu_advance(MachineState& cpu, int ticks) {
	cpu.clocks += OSC1_SPEED / TICK_SPEED * ticks;

	while (cpu.clocks > 0) {
		cpu_step(cpu);
	}

	return true;
}

uint8_t cpu_read_reg(MachineState& cpu, uint32_t address) {
	switch (address) {
	case 0x2008 ... 0x200B:
		return rtc_read_reg(cpu, address);
	case 0x2020 ... 0x202A:
		return irq_read_reg(cpu, address);
	case 0x2040 ... 0x2041:
		return tim256_read_reg(cpu, address);
	case 0x20FE ... 0x20FF:
		return lcd_read_reg(cpu, address);
	default:
		dprintf("Unhandled register read %x", address);
		return cpu.bus_cap;
	}
}

void cpu_write_reg(MachineState& cpu, uint8_t data, uint32_t address) {
	switch (address) {
	case 0x2008 ... 0x200B:
		rtc_write_reg(cpu, data, address);
		break ;
	case 0x2020 ... 0x202A:
		irq_write_reg(cpu, data, address);
		break ;
	case 0x2040 ... 0x2041:
		tim256_write_reg(cpu, data, address);
		break ;
	case 0x20FE ... 0x20FF:
		lcd_write_reg(cpu, data, address);
		break ;
	default:
		dprintf("Unhandled register write %x: %x", address, data);
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
		return cpu_read8(cpu, (cpu.reg.cb << 15) | (address & 0x7FFF));
	} else {
		return cpu_read8(cpu, address);
	}
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
