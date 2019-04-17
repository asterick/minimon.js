#include <stdint.h>

#include "system.h"

/**
 * S1C88 Effective address calculations
 **/

static inline uint32_t calc_vect(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_ind16(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_absHL(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_absIX(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_absIY(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_absBR(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_indDSP(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_indDIX(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_indDIY(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_indIIX(ProcessorState& cpu) {
	// TODO
	return 0;
}

static inline uint32_t calc_indIIY(ProcessorState& cpu) {
	// TODO
	return 0;
}

/**
 * S1C88 Immediate values
 **/

static inline uint8_t cpu_imm8(ProcessorState& cpu) {
	return cpu_read8(cpu, cpu.reg.pc++);
}

static inline uint16_t cpu_imm16(ProcessorState& cpu) {
	return cpu_read8(cpu, cpu.reg.pc++);
}

/**
 * S1C88 Basic intruction operations
 **/

static inline void op_ld8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	// TODO
}

static inline void op_add8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	// TODO
}

static inline void op_adc8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	// TODO
}

static inline void op_sub8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	// TODO
}

static inline void op_sbc8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	// TODO
}

static inline void op_and8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	// TODO
}

static inline void op_or8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	// TODO
}

static inline void op_xor8(ProcessorState& cpu, uint8_t& t, uint8_t s) {
	// TODO
}

static inline void op_cp8(ProcessorState& cpu, uint8_t t, uint8_t s) {
	// TODO
}

static inline void op_bit8(ProcessorState& cpu, uint8_t t, uint8_t s) {
	// TODO
}

static inline void op_inc8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_dec8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_sra8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_srl8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_sla8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_sll8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_rr8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_rrc8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_rl8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_rlc8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_push8(ProcessorState& cpu, uint8_t t) {
	// TODO
}

static inline void op_pop8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_cpl8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_neg8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_ex8(ProcessorState& cpu, uint8_t& t, uint8_t& s) {
	// TODO
}

static inline void op_swap8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_cars8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_call8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_jrs8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_djp8(ProcessorState& cpu, uint8_t& t) {
	// TODO
}

static inline void op_ld16(ProcessorState& cpu, uint16_t t, uint16_t s) {
	// TODO
}

static inline void op_add16(ProcessorState& cpu, uint16_t& t, uint16_t s) {
	// TODO
}

static inline void op_adc16(ProcessorState& cpu, uint16_t& t, uint16_t s) {
	// TODO
}

static inline void op_sub16(ProcessorState& cpu, uint16_t& t, uint16_t s) {
	// TODO
}

static inline void op_sbc16(ProcessorState& cpu, uint16_t& t, uint16_t s) {
	// TODO
}

static inline void op_cp16(ProcessorState& cpu, uint16_t t, uint16_t s) {
	// TODO
}

static inline void op_inc16(ProcessorState& cpu, uint16_t& t) {
	// TODO
}

static inline void op_dec16(ProcessorState& cpu, uint16_t& t) {
	// TODO
}

static inline void op_ex16(ProcessorState& cpu, uint16_t& t, uint16_t& s) {
	// TODO
}

static inline void op_push16(ProcessorState& cpu, uint16_t t) {
	// TODO
}

static inline void op_pop16(ProcessorState& cpu, uint16_t& t) {
	// TODO
}

static inline void op_carl16(ProcessorState& cpu, uint16_t& t) {
	// TODO
}

static inline void op_jr16(ProcessorState& cpu, uint16_t& t) {
	// TODO
}

static inline void op_jrl16(ProcessorState& cpu, uint16_t& t) {
	// TODO
}

static inline void op_jp16(ProcessorState& cpu, uint16_t& t) {
	// TODO
}

static inline void op_int16(ProcessorState& cpu, uint16_t& t) {
	// TODO
}

/**
 * Special purpose instructions
 **/

static void inst_extended_ce(ProcessorState& cpu) {
	
}

static void inst_extended_cf(ProcessorState& cpu) {
	
}

static void inst_pack(ProcessorState& cpu) {

}

static void inst_upck(ProcessorState& cpu) {

}

static void inst_ret(ProcessorState& cpu) {

}

static void inst_rete(ProcessorState& cpu) {

}

static void inst_rets(ProcessorState& cpu) {

}

static void inst_nop(ProcessorState& cpu) {

}

static void inst_mul(ProcessorState& cpu) {

}

static void inst_div(ProcessorState& cpu) {

}

static void inst_push_all(ProcessorState& cpu) {

}

static void inst_push_ale(ProcessorState& cpu) {

}

static void inst_pop_all(ProcessorState& cpu) {

}

static void inst_pop_ale(ProcessorState& cpu) {

}

static void inst_sep(ProcessorState& cpu) {

}

static void inst_halt(ProcessorState& cpu) {

}

static void inst_slp(ProcessorState& cpu) {

}

static void inst_undefined(ProcessorState& cpu) {

}

// This is here for my benefit
#include "table.h"
