#include "machine.h"

static const uint8_t bios[0x2000] = {
	#include "bios.h"
};

__attribute__ ((visibility ("default"))) extern "C"
ProcessorState* const get_machine() {
	static ProcessorState state;
	return &state;
}

extern "C" void debug_print(const void* data);

__attribute__ ((visibility ("default"))) extern "C"
bool cpu_advance(ProcessorState& cpu, int ticks) {
	// TODO: PROPER CPU CATCH-UP HERE

	char data[] = "Hello World";
	debug_print(data);

	cpu_step(cpu);

	return true;
}

uint8_t cpu_read_reg(ProcessorState& cpu, uint32_t address) {
	return 0xCD;
}

void cpu_write_reg(ProcessorState& cpu, uint8_t data, uint32_t address) {
}

__attribute__ ((visibility ("default"))) extern "C"
uint8_t cpu_read8(ProcessorState& cpu, uint32_t address) {
	if (address < 0x1000) {
		return bios[address];
	} else if (address < 0x2000) {
		return cpu.ram[address & 0xFFF];
	} else if (address < 0x2100) {
		return cpu_read_reg(cpu, address);
	} else {
		return cpu_read_cart(cpu, address);
	}
}

__attribute__ ((visibility ("default"))) extern "C"
void cpu_write8(ProcessorState& cpu, uint8_t data, uint32_t address) {
	if (address >= 0x2100) {
		cpu_write_cart(cpu, data, address);
	} else if (address > 0x2000) {
		cpu_write_reg(cpu, data, address);
	} else if (address > 0x1000) {
		cpu.ram[address & 0xFFF] = data;
	}
}
