#include <stdint.h>

#include "machine.h"

typedef void (*InstructionCall)(ProcessorState& cpu);

/**
 * S1C88 Effective address calculations
 **/

static inline uint32_t calc_vect(ProcessorState& cpu) {
	return cpu_imm8(cpu);
}

static inline uint32_t calc_ind16(ProcessorState& cpu) {
	return cpu_imm16(cpu) | (cpu.reg.ep << 16);
}

static inline uint32_t calc_absBR(ProcessorState& cpu) {
	return cpu_imm8(cpu) | (cpu.reg.br << 8) | (cpu.reg.ep << 16);
}

static inline uint32_t calc_absHL(ProcessorState& cpu) {
	return cpu.reg.hl | (cpu.reg.ep << 16);
}

static inline uint32_t calc_absIX(ProcessorState& cpu) {
	return cpu.reg.ix | (cpu.reg.xp << 16);
}

static inline uint32_t calc_absIY(ProcessorState& cpu) {
	return cpu.reg.iy | (cpu.reg.yp << 16);
}

static inline uint32_t calc_indDSP(ProcessorState& cpu) {
	return (cpu.reg.sp + (int8_t)cpu_imm8(cpu)) & 0xFFFF;
}

static inline uint32_t calc_indDIX(ProcessorState& cpu) {
	return ((cpu.reg.ix + (int8_t)cpu_imm8(cpu)) & 0xFFFF) | (cpu.reg.xp << 16);
}

static inline uint32_t calc_indDIY(ProcessorState& cpu) {
	return ((cpu.reg.iy + (int8_t)cpu_imm8(cpu)) & 0xFFFF) | (cpu.reg.yp << 16);
}

static inline uint32_t calc_indIIX(ProcessorState& cpu) {
	return ((cpu.reg.ix + (int8_t)cpu.reg.l) & 0xFFFF) | (cpu.reg.xp << 16);
}

static inline uint32_t calc_indIIY(ProcessorState& cpu) {
	return ((cpu.reg.iy + (int8_t)cpu.reg.l) & 0xFFFF) | (cpu.reg.yp << 16);
}

/**
 * S1C88 Transfer operation
 **/

static inline void op_ld8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	t = s;
}

static inline void op_ld16(ProcessorState& cpu, uint16_t& t, uint16_t s) {
	t = s;
}

static inline void op_ex8(ProcessorState& cpu, uint8_t& t, uint8_t& s) {
	auto temp = t;
	t = s;
	s = temp;
}

static inline void op_ex16(ProcessorState& cpu, uint16_t& t, uint16_t& s) {
	auto temp = t;
	t = s;
	s = temp;
}

static inline void op_swap8(ProcessorState& cpu, uint8_t& t) {
	t = (t << 4) | (t >> 4);
}

/**
 * S1C88 Arithmetic Operation Instructions
 **/

static inline void add8(ProcessorState& cpu, uint8_t& t, uint8_t s, int carry) {
	unsigned int uo = t + s + carry;

	if (cpu.reg.flag.u) {
		t <<= 4;
		s <<= 4;
	}

	if (cpu.reg.flag.d) {
		unsigned int h = (t & 0xF) + (s & 0xF) + carry;

		if (h >= 0x0A) uo += 0x6;
		if (uo >= 0xA0) uo += 0x60;

		cpu.reg.flag.n = 0;
		cpu.reg.flag.v = 0;
	} else {
		cpu.reg.flag.v = ((t ^ s) & (t ^ ~uo) & 0x80) != 0;
		cpu.reg.flag.n = (uo & 0x80) != 0;
	}

	t = (uint8_t)uo;
	if (cpu.reg.flag.u) {
		t >>= 4;
	}

	cpu.reg.flag.z = t == 0;
	cpu.reg.flag.c = uo >= 0x100;
}

static inline void sub8(ProcessorState& cpu, uint8_t& t, uint8_t s, int carry) {
	unsigned int uo = t - s - carry;

	if (cpu.reg.flag.u) {
		t <<= 4;
		s <<= 4;
	}

	if (cpu.reg.flag.d) {
		unsigned int h = (t & 0xF) - (s & 0xF) - carry;

		if (h >= 0x10) uo -= 0x6;
		if (uo >= 0xA0) uo -= 0x60;

		cpu.reg.flag.n = 0;
		cpu.reg.flag.v = 0;
	} else {
		cpu.reg.flag.v = ((t ^ ~s) & (t ^ ~uo) & 0x80) != 0;
		cpu.reg.flag.n = (t & 0x80) != 0;
	}

	t = (uint8_t) uo;
	if (cpu.reg.flag.u) {
		t >>= 4;
	}

	cpu.reg.flag.c = uo >= 0x100;
	cpu.reg.flag.z = t == 0;
}

static inline void op_add8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	add8(cpu, t, s, 0);
}

static inline void op_adc8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	add8(cpu, t, s, cpu.reg.flag.c);
}

static inline void op_sub8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	sub8(cpu, t, s, 0);
}

static inline void op_sbc8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	sub8(cpu, t, s, cpu.reg.flag.c);
}

static inline void op_add16(ProcessorState& cpu, uint16_t& t, uint16_t s) {
	unsigned int uo = t + s;

	cpu.reg.flag.v = ((t ^ s) & (~uo ^ t) & 0x80) != 0;
	t = (uint16_t)uo;
	cpu.reg.flag.z = t == 0;
	cpu.reg.flag.c = uo >= 0x10000;
	cpu.reg.flag.n = (t & 0x8000) != 0;
}

static inline void op_adc16(ProcessorState& cpu, uint16_t& t, uint16_t s) {
	unsigned int uo = t + s + cpu.reg.flag.c;

	cpu.reg.flag.v = ((t ^ s) & (~uo ^ t) & 0x8000) != 0;
	t = (uint16_t)uo;
	cpu.reg.flag.z = t == 0;
	cpu.reg.flag.c = uo >= 0x10000;
	cpu.reg.flag.n = (t & 0x8000) != 0;
}

static inline void op_sub16(ProcessorState& cpu, uint16_t& t, uint16_t s) {
	unsigned int uo = t - s;

	cpu.reg.flag.v = ((t ^ ~s) & (~uo ^ t) & 0x8000) != 0;
	t = (uint16_t)uo;
	cpu.reg.flag.z = t == 0;
	cpu.reg.flag.c = uo >= 0x10000;
	cpu.reg.flag.n = (t & 0x8000) != 0;
}

static inline void op_sbc16(ProcessorState& cpu, uint16_t& t, uint16_t s) {
	unsigned int uo = t - s - cpu.reg.flag.c;

	cpu.reg.flag.v = ((t ^ ~s) & (~uo ^ t) & 0x8000) != 0;
	t = (uint16_t)uo;
	cpu.reg.flag.z = t == 0;
	cpu.reg.flag.c = uo >= 0x10000;
	cpu.reg.flag.n = (t & 0x8000) != 0;
}

static inline void op_cp8(ProcessorState& cpu, uint8_t t, uint8_t s) {
	unsigned int uo = t - s;

	cpu.reg.flag.v = ((t ^ ~s) & (~uo ^ t) & 0x80) != 0;
	cpu.reg.flag.z = (uo & 0xFF) == 0;
	cpu.reg.flag.c = uo >= 0x100;
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_cp16(ProcessorState& cpu, uint16_t t, uint16_t s) {
	unsigned int uo = t - s;

	cpu.reg.flag.v = ((t ^ ~s) & (~uo ^ t) & 0x8000) != 0;
	cpu.reg.flag.z = (uo & 0xFFFF) == 0;
	cpu.reg.flag.c = uo >= 0x10000;
	cpu.reg.flag.n = (uo & 0x8000) != 0;
}

static inline void op_inc8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.flag.z = ++t == 0;
}

static inline void op_dec8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.flag.z = --t == 0;
}

static inline void op_inc16(ProcessorState& cpu, uint16_t& t) {
	cpu.reg.flag.z = ++t == 0;
}

static inline void op_dec16(ProcessorState& cpu, uint16_t& t) {
	cpu.reg.flag.z = --t == 0;
}

static inline void op_neg8(ProcessorState& cpu, uint8_t& t) {
	uint8_t temp = 0;
	op_sub8(cpu, temp, t);
	t = temp;
}

static void inst_mul(ProcessorState& cpu) {
	cpu.reg.hl = (int)cpu.reg.l * (int)cpu.reg.a;

	cpu.reg.flag.z = cpu.reg.hl == 0;
	cpu.reg.flag.c = 0;
	cpu.reg.flag.v = 0;
	cpu.reg.flag.n = (cpu.reg.hl & 0x8000) != 0;
}

static void inst_div(ProcessorState& cpu) {
	if (cpu.reg.a == 0) {
		cpu_interrupt(cpu, IRQ_DIV_ZERO, IRC_PRIO_HIGHEST);
		return ;
	}

	int div = cpu.reg.hl / cpu.reg.a;

	cpu.reg.flag.c = 0;
	cpu.reg.flag.z = (div & 0xFF) == 0;
	cpu.reg.flag.n = (div & 0x80) != 0;

	if (div < 0x100) {
		cpu.reg.h = cpu.reg.hl % cpu.reg.a;
		cpu.reg.l = (uint8_t)div;
		cpu.reg.flag.v = 0;
	} else {
		cpu.reg.flag.v = 1;
	}
}

/**
 * S1C88 Logic Operation Instructions
 **/

static inline void op_and8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	t &= s;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_or8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	t |= s;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_xor8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	t ^= s;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_bit8(ProcessorState& cpu, uint8_t t, uint8_t s) {
	auto v = t & s;
	cpu.reg.flag.z = (v == 0);
	cpu.reg.flag.n = (v & 0x80) != 0;
}

static inline void op_cpl8(ProcessorState& cpu, uint8_t& t) {
	t = ~t;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

/**
 * S1C88 Rotate and shift instructions
 **/

static inline void op_sra8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.flag.c = t & 1;
	t = ((int8_t)t) >> 1;
	cpu.reg.flag.v = 0;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_srl8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.flag.c = t & 1;
	t = t >> 1;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = 0;
}

static inline void op_sla8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.flag.c = (t & 0x80) != 0;
	cpu.reg.flag.v = ((t ^ (t << 1)) & 0x80) != 0;
	t = t << 1;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_sll8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.flag.c = (t & 0x80) != 0;
	t = t << 1;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_rr8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.flag.c = (t & 1);
	t = (t >> 1) | (t << 7);
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_rrc8(ProcessorState& cpu, uint8_t& t) {
	auto old = t;
	t = (cpu.reg.flag.c << 7) | (t >> 1);
	cpu.reg.flag.c = old & 1;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_rl8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.flag.c = (t & 0x80) != 0;
	t = (t >> 7) | (t << 1);
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_rlc8(ProcessorState& cpu, uint8_t& t) {
	auto old = t;
	t = cpu.reg.flag.c | (t << 1);
	cpu.reg.flag.c = (old & 0x80) != 0;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

/**
 * S1C88 Stack control operations
 **/

static inline void op_push8(ProcessorState& cpu, uint8_t t) {
	cpu_push8(cpu, t);
}

static inline void op_pop8(ProcessorState& cpu, uint8_t& t) {
	t = cpu_pop8(cpu);
}

static inline void op_push16(ProcessorState& cpu, uint16_t t) {
	cpu_push16(cpu, t);
}

static inline void op_pop16(ProcessorState& cpu, uint16_t& t) {
	t = cpu_pop8(cpu) << 8;
	t |= cpu_pop8(cpu);
}

static inline void inst_push_ip(ProcessorState& cpu) {
	cpu_push8(cpu, cpu.reg.xp);
	cpu_push8(cpu, cpu.reg.yp);
}

static inline void inst_push_all(ProcessorState& cpu) {
	cpu_push16(cpu, cpu.reg.ba);
	cpu_push16(cpu, cpu.reg.hl);
	cpu_push16(cpu, cpu.reg.ix);
	cpu_push16(cpu, cpu.reg.iy);
	cpu_push8(cpu, cpu.reg.br);
}

static void inst_push_ale(ProcessorState& cpu) {
	inst_push_all(cpu);
	inst_push_ip(cpu);
}

static inline void inst_pop_ip(ProcessorState& cpu) {
	cpu.reg.yp = cpu_pop8(cpu);
	cpu.reg.xp = cpu_pop8(cpu);
}

static inline void inst_pop_all(ProcessorState& cpu) {
	cpu.reg.br = cpu_pop8(cpu);
	cpu.reg.iy = cpu_pop16(cpu);
	cpu.reg.ix = cpu_pop16(cpu);
	cpu.reg.hl = cpu_pop16(cpu);
	cpu.reg.ba = cpu_pop16(cpu);
}

static void inst_pop_ale(ProcessorState& cpu) {
	inst_pop_ip(cpu);
	inst_pop_all(cpu);
}

/**
 * S1C88 Branch instructions
 **/

static inline void op_jrs8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.cb = cpu.reg.nb;
	cpu.reg.pc += (int8_t)t - 1;
}

static inline void op_jrl16(ProcessorState& cpu, uint16_t& t) {
	cpu.reg.cb = cpu.reg.nb;
	cpu.reg.pc += t - 1;
}

static inline void op_jp16(ProcessorState& cpu, uint16_t& t) {
	cpu.reg.cb = cpu.reg.nb;
	cpu.reg.pc = t;
}

static inline void op_djr8(ProcessorState& cpu, uint8_t& t) {
	cpu.reg.flag.z = --cpu.reg.b == 0;
	if (cpu.reg.flag.z) {
		cpu.reg.cb = cpu.reg.nb;
		cpu.reg.pc += (int8_t)t - 1;
	}
}

static inline void op_cars8(ProcessorState& cpu, uint8_t& t) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);

	cpu.reg.pc += (int8_t)t - 1;
	cpu.reg.cb = cpu.reg.nb;
}

static inline void op_carl16(ProcessorState& cpu, uint16_t& t) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);

	cpu.reg.pc += (int16_t)t - 1;
	cpu.reg.cb = cpu.reg.nb;
}

static inline void op_call16(ProcessorState& cpu, uint16_t& t) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);

	cpu.reg.pc = t;
	cpu.reg.cb = cpu.reg.nb;
}

static inline void op_int16(ProcessorState& cpu, uint16_t& t) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);
	cpu_push8(cpu, cpu.reg.sc);

	cpu.reg.pc = t;
	cpu.reg.cb = cpu.reg.nb;
}

static void inst_ret(ProcessorState& cpu) {
	cpu.reg.pc = cpu_pop16(cpu);
	cpu.reg.cb = cpu_pop8(cpu);
	cpu.reg.nb = cpu.reg.cb;
}

static void inst_rete(ProcessorState& cpu) {
	cpu.reg.sc = cpu_pop8(cpu);
	cpu.reg.pc = cpu_pop16(cpu);
	cpu.reg.cb = cpu_pop8(cpu);
	cpu.reg.nb = cpu.reg.cb;
}

static void inst_rets(ProcessorState& cpu) {
	cpu.reg.pc = cpu_pop16(cpu);
	cpu.reg.cb = cpu_pop8(cpu);
	cpu.reg.nb = cpu.reg.cb;
	cpu.reg.pc += 2;
}

/**
 * S1C88 Special purpose instructions
 **/

static void inst_nop(ProcessorState& cpu) {

}

static void inst_pack(ProcessorState& cpu) {
	cpu.reg.a = ((cpu.reg.b & 0xF) << 4) | (cpu.reg.a & 0xF);
}

static void inst_upck(ProcessorState& cpu) {
	cpu.reg.b = cpu.reg.a >> 4;
	cpu.reg.a = cpu.reg.a & 0xF;
}

static void inst_sep(ProcessorState& cpu) {
	cpu.reg.b = (cpu.reg.a & 0x80) ? 0xFF : 0x00;
}

static void inst_halt(ProcessorState& cpu) {
	// TODO
	cpu.clocks = 0;
}

static void inst_slp(ProcessorState& cpu) {
	// TODO
	cpu.clocks = 0;
}

static void inst_undefined(ProcessorState& cpu) {
	// TODO
	cpu.clocks = 0;
}

static void inst_extended_ce(ProcessorState& cpu);
static void inst_extended_cf(ProcessorState& cpu);

// Generated compound instructions and tables
#include "table.h"

static void inst_extended_ce(ProcessorState& cpu) {
	inst_table1[cpu_imm8(cpu)](cpu);
}

static void inst_extended_cf(ProcessorState& cpu) {
	inst_table2[cpu_imm8(cpu)](cpu);
}

__attribute__ ((visibility ("default")))
void cpu_step(ProcessorState& cpu) {
	inst_table0[cpu_imm8(cpu)](cpu);
}
