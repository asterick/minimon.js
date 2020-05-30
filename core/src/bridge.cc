#include <stdint.h>
#include <stddef.h>

#include "machine.h"
#include "debug.h"

enum FieldType : uint32_t {
	TYPE_END = 0,
	TYPE_STRUCT,
	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPE_INT8,
	TYPE_INT16,
	TYPE_INT32,
	TYPE_BOOL
};

struct FieldDecl;
struct StructDecl {
	uint32_t			size;
	const FieldDecl* 	fields;
};

struct FieldDecl {
	FieldType 			type;
	const char* 		name;
	uint32_t 			offset;
	const StructDecl*	def;
	const int* 			sizes;
};

#define FIELD(n, s, f, t, ...) 	  { t, n, offsetof(s, f), NULL, __VA_ARGS__ }
#define STRUCT(n, s, f, def, ...) { TYPE_STRUCT, n, offsetof(s, f), &def, __VA_ARGS__ }
#define SIZE(...) 				  ((const int[]){ __VA_ARGS__, -1 })

extern "C" Machine::State* const get_machine() {
	static Machine::State machine_state;
	return &machine_state;
}

static const StructDecl CpuState = {
	sizeof(CPU::State),
	(const FieldDecl[]) {
		FIELD("sc", CPU::State, sc,  TYPE_UINT8),
		FIELD("cc", CPU::State, cc,  TYPE_UINT8),
		FIELD("br", CPU::State, br,  TYPE_UINT8), 
		FIELD("ep", CPU::State, ep,  TYPE_UINT8), 
		FIELD("xp", CPU::State, xp,  TYPE_UINT8), 
		FIELD("yp", CPU::State, yp,  TYPE_UINT8), 
		FIELD("cb", CPU::State, cb,  TYPE_UINT8), 
		FIELD("nb", CPU::State, nb,  TYPE_UINT8), 
		FIELD( "a", CPU::State,  a,  TYPE_UINT8),
		FIELD( "b", CPU::State,  b,  TYPE_UINT8),
		FIELD( "l", CPU::State,  l,  TYPE_UINT8),
		FIELD( "h", CPU::State,  h,  TYPE_UINT8),
		FIELD("ba", CPU::State, ba, TYPE_UINT16),
		FIELD("hl", CPU::State, hl, TYPE_UINT16),
		FIELD("pc", CPU::State, pc, TYPE_UINT16),
		FIELD("sp", CPU::State, sp, TYPE_UINT16),
		FIELD("ix", CPU::State, ix, TYPE_UINT16),
		FIELD("iy", CPU::State, iy, TYPE_UINT16),
		{ TYPE_END }
	}
};

static const StructDecl IrqState = {
	sizeof(IRQ::State),
	(const FieldDecl[]) {
		FIELD("enable", IRQ::State, enable, TYPE_BOOL),
		FIELD("active", IRQ::State, active, TYPE_BOOL),
		FIELD("priorty", IRQ::State, priority, TYPE_UINT8, SIZE(IRQ::TOTAL_HARDWARE_IRQS)),
		FIELD("next_priority", IRQ::State, next_priority, TYPE_INT32),
		FIELD("next_irq", IRQ::State, next_irq, TYPE_INT8),
		{ TYPE_END }
	}
};


static const StructDecl Tim256State = {
	sizeof(TIM256::State),
	(const FieldDecl[]) {
		FIELD("running", TIM256::State, running, TYPE_BOOL),
		FIELD("value", TIM256::State, value, TYPE_UINT16),
		{ TYPE_END }
	}
};

static const StructDecl RtcState = {
	sizeof(RTC::State),
	(const FieldDecl[]) {
		FIELD("running", RTC::State, running, TYPE_BOOL),
		FIELD("value", RTC::State, value, TYPE_UINT32),
		FIELD("prescale", RTC::State, prescale, TYPE_UINT16),
		{ TYPE_END }
	}
};


static const StructDecl ControlState = {
	sizeof(Control::State),
	(const FieldDecl[]) {
		FIELD("data", Control::State, data, TYPE_UINT8, SIZE(3)),
		{ TYPE_END }
	}
};

static const StructDecl EepromState = {
	sizeof(EEPROM::State),
	(const FieldDecl[]) {
		FIELD("data", EEPROM::State, data,	TYPE_UINT8, SIZE(0x2000)),
		//I2C_MODE mode;
		//uint8_t shift;
		//int bit;
		//unsigned address:13;

		//PinState data_in;
		//PinState data_out;
		//PinState clock_in;
		{ TYPE_END }
	}
};

static const StructDecl GpioState = {
	sizeof(GPIO::State),
	(const FieldDecl[]) {
		FIELD("output", 	GPIO::State, output,    TYPE_UINT8),
		FIELD("direction", 	GPIO::State, direction, TYPE_UINT8),
		STRUCT("eeprom", 	GPIO::State, eeprom, 	EepromState),
		{ TYPE_END }
	}
};

static const StructDecl MachineState = {
	sizeof(Machine::State),
	(const FieldDecl[]) {
		STRUCT("cpu",  Machine::State, reg, CpuState),
		STRUCT("ctrl", Machine::State, ctrl, ControlState),
		STRUCT("gpio", Machine::State, gpio, GpioState),
		STRUCT("rtc",  Machine::State, rtc, RtcState),
		STRUCT("irq",  Machine::State, irq, IrqState),
		STRUCT("tim256",  Machine::State, tim256, Tim256State),
		FIELD("ram",   Machine::State, ram, TYPE_UINT8, SIZE(0x1000)),

		//Timers::State timers;
		//Input::State input;
		//LCD::State lcd;
		//Blitter::State blitter;
		//Blitter::Overlay overlay;

		FIELD("bus_cap", Machine::State, bus_cap, TYPE_UINT8),
		FIELD("clocks", Machine::State, clocks, TYPE_INT32),
		FIELD("osc1_overflow", Machine::State, osc1_overflow, TYPE_INT32),
		FIELD("status", Machine::State, status, TYPE_UINT8),

		{ TYPE_END }
	}
};

extern "C" const StructDecl* get_description() {
	return &MachineState;
}
