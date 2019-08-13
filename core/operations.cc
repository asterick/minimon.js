/*
ISC License

Copyright (c) 2019, Bryon Vandiver

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdint.h>

#include "machine.h"

/**
 * S1C88 Effective address calculations
 **/

static inline uint32_t calc_vect(Machine::State& cpu) {
	return cpu_imm8(cpu);
}

static inline uint32_t calc_ind16(Machine::State& cpu) {
	return (cpu.reg.ep << 16) | cpu_imm16(cpu);
}

static inline uint32_t calc_absBR(Machine::State& cpu) {
	return (cpu.reg.ep << 16) | (cpu.reg.br << 8) | cpu_imm8(cpu);
}

static inline uint32_t calc_absHL(Machine::State& cpu) {
	return (cpu.reg.ep << 16) | cpu.reg.hl;
}

static inline uint32_t calc_absIX(Machine::State& cpu) {
	return (cpu.reg.xp << 16) | cpu.reg.ix;
}

static inline uint32_t calc_absIY(Machine::State& cpu) {
	return (cpu.reg.yp << 16) | cpu.reg.iy;
}

static inline uint32_t calc_indDSP(Machine::State& cpu) {
	return (cpu.reg.sp + (int8_t)cpu_imm8(cpu)) & 0xFFFF;
}

static inline uint32_t calc_indDIX(Machine::State& cpu) {
	return (cpu.reg.xp << 16) | ((cpu.reg.ix + (int8_t)cpu_imm8(cpu)) & 0xFFFF);
}

static inline uint32_t calc_indDIY(Machine::State& cpu) {
	return (cpu.reg.yp << 16) | ((cpu.reg.iy + (int8_t)cpu_imm8(cpu)) & 0xFFFF);
}

static inline uint32_t calc_indIIX(Machine::State& cpu) {
	return (cpu.reg.xp << 16) | ((cpu.reg.ix + (int8_t)cpu.reg.l) & 0xFFFF);
}

static inline uint32_t calc_indIIY(Machine::State& cpu) {
	return (cpu.reg.yp << 16) | ((cpu.reg.iy + (int8_t)cpu.reg.l) & 0xFFFF);
}


/**
 * Instruction templates
 **/

static inline uint8_t add8(Machine::State& cpu, uint8_t t, uint8_t s, int carry) {
	if (cpu.reg.flag.u) {
		t <<= 4;
		s <<= 4;
		carry <<= 4;
	}

	int o = t + s + carry;

	if (cpu.reg.flag.d) {
		int h = (t & 0xF) + (s & 0xF) + carry;

		if (h > 0x09) o += 0x6;
		if (o > 0x90) o += 0x60;

		cpu.reg.flag.v = 0;
		cpu.reg.flag.n = 0;
	} else {
		cpu.reg.flag.v = ((t ^ ~s) & (t ^ o) & 0x80) != 0;
		cpu.reg.flag.n = (o & 0x80) != 0;
	}

	cpu.reg.flag.c = o >= 0x100;
	cpu.reg.flag.z = (o & 0xFF) == 0;

	if (cpu.reg.flag.u) {
		return (o & 0xF0) >> 4;
	} else {
		return o & 0xFF;
	}
}

static inline uint8_t sub8(Machine::State& cpu, uint8_t t, uint8_t s, int carry) {
	if (cpu.reg.flag.u) {
		t <<= 4;
		s <<= 4;
		carry <<= 4;
	}

	int o = t - s - carry;

	if (cpu.reg.flag.d) {
		int h = (t & 0xF) - (s & 0xF) - carry;

		if (h < 0) o -= 0x6;
		if (o < 0) o -= 0x60;

		cpu.reg.flag.v = 0;
		cpu.reg.flag.n = 0;
	} else {
		cpu.reg.flag.v = ((t ^ s) & (t ^ o) & 0x80) != 0;
		cpu.reg.flag.n = (t & 0x80) != 0;
	}

	cpu.reg.flag.c = o < 0;
	cpu.reg.flag.z = (o & 0xFF) == 0;

	if (cpu.reg.flag.u) {
		return (o & 0xF0) >> 4;
	} else {
		return o & 0xFF;
	}
}

/**
 * S1C88 Transfer operation
 **/

static inline void op_ld8(Machine::State& cpu, uint8_t& t, uint8_t s) {
	t = s;
}

static inline void op_ex8(Machine::State& cpu, uint8_t& t, uint8_t& s) {
	auto temp = t;
	t = s;
	s = temp;
}

static inline void op_swap8(Machine::State& cpu, uint8_t& t) {
	t = (t << 4) | (t >> 4);
}

static inline void op_ld16(Machine::State& cpu, uint16_t& t, uint16_t s) {
	t = s;
}

static inline void op_ex16(Machine::State& cpu, uint16_t& t, uint16_t& s) {
	auto temp = t;
	t = s;
	s = temp;
}

/**
 * 8-bit Arithmetic Operation Instructions
 **/

static inline void op_add8(Machine::State& cpu, uint8_t& t, uint8_t s) {
	t = add8(cpu, t, s, 0);
}

static inline void op_adc8(Machine::State& cpu, uint8_t& t, uint8_t s) {
	t = add8(cpu, t, s, cpu.reg.flag.c);
}

static inline void op_sub8(Machine::State& cpu, uint8_t& t, uint8_t s) {
	t = sub8(cpu, t, s, 0);
}

static inline void op_sbc8(Machine::State& cpu, uint8_t& t, uint8_t s) {
	t = sub8(cpu, t, s, cpu.reg.flag.c);
}

static inline void op_and8(Machine::State& cpu, uint8_t& t, uint8_t s) {
	uint8_t out = t & s;
	cpu.reg.flag.z = (out == 0);
	cpu.reg.flag.n = (out & 0x80) != 0;
	t = out;
}

static inline void op_or8(Machine::State& cpu, uint8_t& t, uint8_t s) {
	uint8_t out = t | s;
	cpu.reg.flag.z = (out == 0);
	cpu.reg.flag.n = (out & 0x80) != 0;
	t = out;
}

static inline void op_xor8(Machine::State& cpu, uint8_t& t, uint8_t s) {
	uint8_t out = t ^ s;
	cpu.reg.flag.z = (out == 0);
	cpu.reg.flag.n = (out & 0x80) != 0;
	t = out;
}

static inline void op_cp8(Machine::State& cpu, uint8_t t, uint8_t s) {
	int uo = t - s;

	cpu.reg.flag.v = ((t ^ s) & (t ^ uo) & 0x80) != 0;
	cpu.reg.flag.z = (uo & 0xFF) == 0;
	cpu.reg.flag.c = uo < 0;
	cpu.reg.flag.n = (uo & 0x80) != 0;
}

static inline void op_bit8(Machine::State& cpu, uint8_t t, uint8_t s) {
	auto v = t & s;
	cpu.reg.flag.z = (v == 0);
	cpu.reg.flag.n = (v & 0x80) != 0;
}

static inline void op_inc8(Machine::State& cpu, uint8_t& t) {
	cpu.reg.flag.z = (0 == ++t);
}

static inline void op_dec8(Machine::State& cpu, uint8_t& t) {
	cpu.reg.flag.z = (0 == --t);
}

static inline void op_cpl8(Machine::State& cpu, uint8_t& t) {
	t = ~t;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_neg8(Machine::State& cpu, uint8_t& t) {
	t = sub8(cpu, 0, t, 0);
}

static inline void inst_mlt(Machine::State& cpu) {
	cpu.reg.hl = (unsigned int)cpu.reg.l * (unsigned int)cpu.reg.a;

	cpu.reg.flag.z = cpu.reg.hl == 0;
	cpu.reg.flag.c = 0;
	cpu.reg.flag.v = 0;
	cpu.reg.flag.n = (cpu.reg.hl & 0x8000) != 0;
}

static inline void inst_div(Machine::State& cpu) {
	if (cpu.reg.a == 0) {
		IRQ::trigger(cpu, IRQ::IRQ_DIV_ZERO);
		return ;
	}

	int div = cpu.reg.hl / cpu.reg.a;

	cpu.reg.flag.c = 0;

	if (div < 0x100) {
		cpu.reg.h = cpu.reg.hl % cpu.reg.a;
		cpu.reg.l = (uint8_t)div;
		cpu.reg.flag.n = (div & 0x80) != 0;
		cpu.reg.flag.z = cpu.reg.l == 0;	// Not sure when this is calculated
		cpu.reg.flag.v = 0;
	} else {
		cpu.reg.flag.n = 1;
		cpu.reg.flag.v = 1;
	}
}

/**
 * 16-bit Arithmetic Operation Instructions
 **/

static inline void op_add16(Machine::State& cpu, uint16_t& t, uint16_t s) {
	int uo = t + s;

	cpu.reg.flag.v = ((t ^ ~s) & (t ^ uo) & 0x8000) != 0;
	cpu.reg.flag.c = uo >= 0x10000;
	cpu.reg.flag.n = (uo & 0x8000) != 0;
	t = (uint16_t)uo;
	cpu.reg.flag.z = t == 0;
}

static inline void op_adc16(Machine::State& cpu, uint16_t& t, uint16_t s) {
	int uo = t + s + cpu.reg.flag.c;

	cpu.reg.flag.v = ((t ^ ~s) & (t ^ uo) & 0x8000) != 0;
	cpu.reg.flag.c = uo >= 0x10000;
	cpu.reg.flag.n = (uo & 0x8000) != 0;
	t = (uint16_t)uo;
	cpu.reg.flag.z = t == 0;
}

static inline void op_sub16(Machine::State& cpu, uint16_t& t, uint16_t s) {
	int uo = t - s;

	cpu.reg.flag.v = ((t ^ s) & (t ^ uo) & 0x8000) != 0;
	cpu.reg.flag.c = uo < 0;
	cpu.reg.flag.n = (uo & 0x8000) != 0;
	t = (uint16_t)uo;
	cpu.reg.flag.z = t == 0;
}

static inline void op_sbc16(Machine::State& cpu, uint16_t& t, uint16_t s) {
	int uo = t - s - cpu.reg.flag.c;

	cpu.reg.flag.v = ((t ^ s) & (t ^ uo) & 0x8000) != 0;
	cpu.reg.flag.c = uo < 0;
	cpu.reg.flag.n = (uo & 0x8000) != 0;
	t = (uint16_t)uo;
	cpu.reg.flag.z = t == 0;
}

static inline void op_cp16(Machine::State& cpu, uint16_t t, uint16_t s) {
	int uo = t - s;

	cpu.reg.flag.v = ((t ^ s) & (t ^ uo) & 0x8000) != 0;
	cpu.reg.flag.c = uo < 0;
	cpu.reg.flag.n = (uo & 0x8000) != 0;
	t = (uint16_t)uo;
	cpu.reg.flag.z = t == 0;
}

static inline void op_inc16(Machine::State& cpu, uint16_t& t) {
	cpu.reg.flag.z = (0 == ++t);
}

static inline void op_dec16(Machine::State& cpu, uint16_t& t) {
	cpu.reg.flag.z = (0 == --t);
}

/**
 * S1C88 Auxiliary Operation Instructions
 **/

static inline void inst_pack(Machine::State& cpu) {
	cpu.reg.a = (cpu.reg.b << 4) | (cpu.reg.a & 0xF);
}

static inline void inst_upck(Machine::State& cpu) {
	cpu.reg.b = cpu.reg.a >> 4;
	cpu.reg.a = cpu.reg.a & 0xF;
}

static inline void inst_sep(Machine::State& cpu) {
	cpu.reg.b = (cpu.reg.a & 0x80) ? 0xFF : 0x00;
}

/**
 * S1C88 Rotate and shift instructions (unverified)
 **/

static inline void op_rl8(Machine::State& cpu, uint8_t& t) {
	auto old = t;
	t = (t << 1) | cpu.reg.flag.c;
	cpu.reg.flag.c = (old & 0x80) != 0;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_rlc8(Machine::State& cpu, uint8_t& t) {
	cpu.reg.flag.c = (t & 0x80) != 0;
	t = (t << 1) | (t >> 7);
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_rr8(Machine::State& cpu, uint8_t& t) {
	auto old = t;
	t = (t >> 1) | (cpu.reg.flag.c << 7);
	cpu.reg.flag.c = old & 1;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_rrc8(Machine::State& cpu, uint8_t& t) {
	cpu.reg.flag.c = (t & 1);
	t = (t >> 1) | (t << 7);
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_sla8(Machine::State& cpu, uint8_t& t) {
	cpu.reg.flag.c = (t & 0x80) != 0;
	cpu.reg.flag.v = ((t ^ (t << 1)) & 0x80) != 0;
	t = t << 1;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_sll8(Machine::State& cpu, uint8_t& t) {
	cpu.reg.flag.c = (t & 0x80) != 0;
	t = t << 1;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_sra8(Machine::State& cpu, uint8_t& t) {
	cpu.reg.flag.c = t & 1;
	t = (t >> 1) | (t & 0x80);
	cpu.reg.flag.v = 0;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = (t & 0x80) != 0;
}

static inline void op_srl8(Machine::State& cpu, uint8_t& t) {
	cpu.reg.flag.c = t & 1;
	t = t >> 1;
	cpu.reg.flag.z = (t == 0);
	cpu.reg.flag.n = 0;
}

/**
 * S1C88 Stack control operations
 **/

static inline void op_push8(Machine::State& cpu, uint8_t t) {
	cpu_push8(cpu, t);
}

static inline void op_push16(Machine::State& cpu, uint16_t t) {
	cpu_push16(cpu, t);
}

static inline void inst_push_ip(Machine::State& cpu) {
	cpu_push8(cpu, cpu.reg.xp);
	cpu_push8(cpu, cpu.reg.yp);
}

static inline void inst_push_all(Machine::State& cpu) {
	cpu_push16(cpu, cpu.reg.ba);
	cpu_push16(cpu, cpu.reg.hl);
	cpu_push16(cpu, cpu.reg.ix);
	cpu_push16(cpu, cpu.reg.iy);
	cpu_push8(cpu, cpu.reg.br);
}

static inline void inst_push_ale(Machine::State& cpu) {
	inst_push_all(cpu);
	op_push8(cpu, cpu.reg.ep);
	inst_push_ip(cpu);
}

static inline void op_pop8(Machine::State& cpu, uint8_t& t) {
	t = cpu_pop8(cpu);
}

static inline void op_pop16(Machine::State& cpu, uint16_t& t) {
	t = cpu_pop16(cpu);
}

static inline void inst_pop_ip(Machine::State& cpu) {
	cpu.reg.yp = cpu_pop8(cpu);
	cpu.reg.xp = cpu_pop8(cpu);
}

static inline void inst_pop_all(Machine::State& cpu) {
	cpu.reg.br = cpu_pop8(cpu);
	cpu.reg.iy = cpu_pop16(cpu);
	cpu.reg.ix = cpu_pop16(cpu);
	cpu.reg.hl = cpu_pop16(cpu);
	cpu.reg.ba = cpu_pop16(cpu);
}

static inline void inst_pop_ale(Machine::State& cpu) {
	inst_pop_ip(cpu);
	cpu.reg.ep = cpu_pop8(cpu);
	inst_pop_all(cpu);
}

/**
 * S1C88 Branch instructions
 **/

static inline void op_jrs8(Machine::State& cpu, uint8_t t) {
	cpu.reg.cb = cpu.reg.nb;
	cpu.reg.pc += (int8_t)t - 1;
}

static inline void op_jrl16(Machine::State& cpu, uint16_t t) {
	cpu.reg.cb = cpu.reg.nb;
	cpu.reg.pc += t - 1;
}

static inline void inst_djr_nz_rr(Machine::State& cpu) {
	int8_t off = cpu_imm8(cpu);

	cpu.reg.flag.z = 0 == --cpu.reg.b;
	if (!cpu.reg.flag.z) {
		cpu.reg.cb = cpu.reg.nb;
		cpu.reg.pc += off - 1;
	}
}

static inline void op_jp16(Machine::State& cpu, uint16_t t) {
	cpu.reg.pc = t;
	cpu.reg.cb = cpu.reg.nb;
}

static inline void op_cars8(Machine::State& cpu, uint8_t t) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);

	cpu.reg.pc += (int8_t)t - 1;
	cpu.reg.cb = cpu.reg.nb;
}

static inline void op_carl16(Machine::State& cpu, uint16_t t) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);

	cpu.reg.pc += t - 1;
	cpu.reg.cb = cpu.reg.nb;
}

static inline void op_call16(Machine::State& cpu, uint16_t t) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);

	cpu.reg.pc = t;
	cpu.reg.cb = cpu.reg.nb;
}

static inline void op_int16(Machine::State& cpu, uint16_t t) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);
	cpu_push8(cpu, cpu.reg.sc);

	cpu.reg.pc = t;
	cpu.reg.cb = cpu.reg.nb;
}

static inline void inst_ret(Machine::State& cpu) {
	cpu.reg.pc = cpu_pop16(cpu);
	cpu.reg.nb = cpu.reg.cb = cpu_pop8(cpu);
}

static inline void op_rete8(Machine::State& cpu) {
	cpu.reg.sc = cpu_pop8(cpu);
	cpu.reg.pc = cpu_pop16(cpu);
	cpu.reg.nb = cpu.reg.cb = cpu_pop8(cpu);
}

static inline void inst_rets(Machine::State& cpu) {
	cpu.reg.pc = cpu_pop16(cpu);
	cpu.reg.nb = cpu.reg.cb = cpu_pop8(cpu);
	cpu.reg.pc += 2;
}

/**
 * System Control Instructions
 **/

static void inst_nop(Machine::State& cpu) {

}

static void inst_halt(Machine::State& cpu) {
	cpu.status = Machine::STATUS_HALTED;
}

static void inst_slp(Machine::State& cpu) {
	cpu.status = Machine::STATUS_SLEEPING;
}

static int inst_undefined(Machine::State& cpu) {
	// This is not strictly correct, but we want to treat this like it's a bad thing
	cpu.status = Machine::STATUS_CRASHED;
	return 0;
}

// Generated compound instructions and tables
#include "table.h"
