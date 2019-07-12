void inst_add_a_a(ProcessorState& cpu) {
	op_add8(cpu, cpu.reg.a, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_add_a_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_ba_ba(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.ba, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_add_a_b(ProcessorState& cpu) {
	op_add8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_add_a_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_ba_hl(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.ba, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_add_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_add_a_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_ba_ix(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.ba, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_add_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_add_a_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_ba_iy(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.ba, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_add_a_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 3;
}

void inst_add_abshl_a(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_add8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_adc_ba_ba(ProcessorState& cpu) {
	op_adc16(cpu, cpu.reg.ba, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_add_a_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_add8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_adc_ba_hl(ProcessorState& cpu) {
	op_adc16(cpu, cpu.reg.ba, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_add_a_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_add_abshl_absix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_adc_ba_ix(ProcessorState& cpu) {
	op_adc16(cpu, cpu.reg.ba, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_add_a_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_add_abshl_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_add8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_adc_ba_iy(ProcessorState& cpu) {
	op_adc16(cpu, cpu.reg.ba, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_adc_a_a(ProcessorState& cpu) {
	op_adc8(cpu, cpu.reg.a, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_adc_a_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_ba_ba(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.ba, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_adc_a_b(ProcessorState& cpu) {
	op_adc8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_adc_a_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_ba_hl(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.ba, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_adc_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_adc_a_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_ba_ix(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.ba, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_adc_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_adc_a_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_ba_iy(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.ba, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_adc_a_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 3;
}

void inst_adc_abshl_a(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_adc8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_sbc_ba_ba(ProcessorState& cpu) {
	op_sbc16(cpu, cpu.reg.ba, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_adc_a_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_adc_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_adc8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_ba_hl(ProcessorState& cpu) {
	op_sbc16(cpu, cpu.reg.ba, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_adc_a_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_adc_abshl_absix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_ba_ix(ProcessorState& cpu) {
	op_sbc16(cpu, cpu.reg.ba, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_adc_a_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_adc_abshl_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_adc8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_ba_iy(ProcessorState& cpu) {
	op_sbc16(cpu, cpu.reg.ba, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_sub_a_a(ProcessorState& cpu) {
	op_sub8(cpu, cpu.reg.a, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_sub_a_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_a_b(ProcessorState& cpu) {
	op_sub8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_sub_a_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_sub_a_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_sub_a_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_a_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 3;
}

void inst_sub_abshl_a(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_sub8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_sub_a_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_sub8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sub_a_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_sub_abshl_absix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sub_a_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_sub_abshl_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sub8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_a_a(ProcessorState& cpu) {
	op_sbc8(cpu, cpu.reg.a, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_sbc_a_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_ba_ba(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.ba, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_sbc_a_b(ProcessorState& cpu) {
	op_sbc8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_sbc_a_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_ba_hl(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.ba, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_sbc_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_sbc_a_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_ba_ix(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.ba, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_sbc_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_sbc_a_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_ba_iy(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.ba, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_sbc_a_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 3;
}

void inst_sbc_abshl_a(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_sbc8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_sbc_a_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sbc_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_sbc8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_a_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_sbc_abshl_absix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_a_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_sbc_abshl_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_sbc8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_and_a_a(ProcessorState& cpu) {
	op_and8(cpu, cpu.reg.a, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_and_a_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_hl_ba(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.hl, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_and_a_b(ProcessorState& cpu) {
	op_and8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_and_a_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_hl_hl(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.hl, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_and_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_and_a_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_hl_ix(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.hl, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_and_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_and_a_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_hl_iy(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.hl, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_and_a_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 3;
}

void inst_and_abshl_a(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_and8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_adc_hl_ba(ProcessorState& cpu) {
	op_adc16(cpu, cpu.reg.hl, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_and_a_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_and_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_and8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_adc_hl_hl(ProcessorState& cpu) {
	op_adc16(cpu, cpu.reg.hl, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_and_a_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_and_abshl_absix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_adc_hl_ix(ProcessorState& cpu) {
	op_adc16(cpu, cpu.reg.hl, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_and_a_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_and_abshl_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_and8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_adc_hl_iy(ProcessorState& cpu) {
	op_adc16(cpu, cpu.reg.hl, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_or_a_a(ProcessorState& cpu) {
	op_or8(cpu, cpu.reg.a, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_or_a_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_hl_ba(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.hl, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_or_a_b(ProcessorState& cpu) {
	op_or8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_or_a_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_hl_hl(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.hl, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_or_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_or_a_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_hl_ix(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.hl, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_or_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_or_a_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_sub_hl_iy(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.hl, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_or_a_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 3;
}

void inst_or_abshl_a(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_or8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_sbc_hl_ba(ProcessorState& cpu) {
	op_sbc16(cpu, cpu.reg.hl, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_or_a_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_or_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_or8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_hl_hl(ProcessorState& cpu) {
	op_sbc16(cpu, cpu.reg.hl, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_or_a_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_or_abshl_absix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_hl_ix(ProcessorState& cpu) {
	op_sbc16(cpu, cpu.reg.hl, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_or_a_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_or_abshl_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_or8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_hl_iy(ProcessorState& cpu) {
	op_sbc16(cpu, cpu.reg.hl, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_cp_a_a(ProcessorState& cpu) {
	op_cp8(cpu, cpu.reg.a, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_cp_a_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_a_b(ProcessorState& cpu) {
	op_cp8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_cp_a_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_cp_a_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_cp_a_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_a_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 3;
}

void inst_cp_abshl_a(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_cp8(cpu, data0, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_cp_a_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_cp8(cpu, data0, data1);
	cpu.clocks += 4;
}

void inst_cp_a_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_cp_abshl_absix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, data0, data1);
	cpu.clocks += 4;
}

void inst_cp_a_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_cp_abshl_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_cp8(cpu, data0, data1);
	cpu.clocks += 4;
}

void inst_xor_a_a(ProcessorState& cpu) {
	op_xor8(cpu, cpu.reg.a, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_xor_a_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_hl_ba(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.hl, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_xor_a_b(ProcessorState& cpu) {
	op_xor8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_xor_a_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_hl_hl(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.hl, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_xor_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_xor_a_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_hl_ix(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.hl, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_xor_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_xor_a_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_cp_hl_iy(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.hl, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_xor_a_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 3;
}

void inst_xor_abshl_a(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_xor8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_xor_a_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_xor_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_xor8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_xor_a_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_xor_abshl_absix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_xor_a_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_xor_abshl_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_xor8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_a_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.a);
	cpu.clocks += 1;
}

void inst_ld_a_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_ix_ba(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.ix, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_ld_a_b(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 1;
}

void inst_ld_a_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_ix_hl(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.ix, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_ld_a_l(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.l);
	cpu.clocks += 1;
}

void inst_ld_a_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_iy_ba(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.iy, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_ld_a_h(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.h);
	cpu.clocks += 1;
}

void inst_ld_a_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 4;
}

void inst_add_iy_hl(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.iy, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_ld_a_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 3;
}

void inst_ld_inddix_a(ProcessorState& cpu) {
	auto addr0 = calc_indDIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_add_sp_ba(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.sp, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_ld_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_ld_inddiy_a(ProcessorState& cpu) {
	auto addr0 = calc_indDIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_add_sp_hl(ProcessorState& cpu) {
	op_add16(cpu, cpu.reg.sp, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_ld_a_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_ld_indiix_a(ProcessorState& cpu) {
	auto addr0 = calc_indIIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_a_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_ld_indiiy_a(ProcessorState& cpu) {
	auto addr0 = calc_indIIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_b_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.b, cpu.reg.a);
	cpu.clocks += 1;
}

void inst_ld_b_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 4;
}

void inst_sub_ix_ba(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.ix, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_ld_b_b(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.b, cpu.reg.b);
	cpu.clocks += 1;
}

void inst_ld_b_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 4;
}

void inst_sub_ix_hl(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.ix, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_ld_b_l(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.b, cpu.reg.l);
	cpu.clocks += 1;
}

void inst_ld_b_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 4;
}

void inst_sub_iy_ba(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.iy, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_ld_b_h(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.b, cpu.reg.h);
	cpu.clocks += 1;
}

void inst_ld_b_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 4;
}

void inst_sub_iy_hl(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.iy, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_ld_b_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 3;
}

void inst_ld_inddix_b(ProcessorState& cpu) {
	auto addr0 = calc_indDIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.b);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_sub_sp_ba(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.sp, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_ld_b_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 2;
}

void inst_ld_inddiy_b(ProcessorState& cpu) {
	auto addr0 = calc_indDIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.b);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_sub_sp_hl(ProcessorState& cpu) {
	op_sub16(cpu, cpu.reg.sp, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_ld_b_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 2;
}

void inst_ld_indiix_b(ProcessorState& cpu) {
	auto addr0 = calc_indIIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.b);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_b_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 2;
}

void inst_ld_indiiy_b(ProcessorState& cpu) {
	auto addr0 = calc_indIIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.b);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_l_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.l, cpu.reg.a);
	cpu.clocks += 1;
}

void inst_ld_l_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 4;
}

void inst_ld_l_b(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.l, cpu.reg.b);
	cpu.clocks += 1;
}

void inst_ld_l_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 4;
}

void inst_ld_l_l(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.l, cpu.reg.l);
	cpu.clocks += 1;
}

void inst_ld_l_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 4;
}

void inst_ld_l_h(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.l, cpu.reg.h);
	cpu.clocks += 1;
}

void inst_ld_l_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 4;
}

void inst_ld_l_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 3;
}

void inst_ld_inddix_l(ProcessorState& cpu) {
	auto addr0 = calc_indDIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.l);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_l_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 2;
}

void inst_ld_inddiy_l(ProcessorState& cpu) {
	auto addr0 = calc_indDIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.l);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_l_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 2;
}

void inst_ld_indiix_l(ProcessorState& cpu) {
	auto addr0 = calc_indIIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.l);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_l_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 2;
}

void inst_ld_indiiy_l(ProcessorState& cpu) {
	auto addr0 = calc_indIIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.l);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_h_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.h, cpu.reg.a);
	cpu.clocks += 1;
}

void inst_ld_h_inddix(ProcessorState& cpu) {
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 4;
}

void inst_ld_h_b(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.h, cpu.reg.b);
	cpu.clocks += 1;
}

void inst_ld_h_inddiy(ProcessorState& cpu) {
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 4;
}

void inst_ld_h_l(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.h, cpu.reg.l);
	cpu.clocks += 1;
}

void inst_ld_h_indiix(ProcessorState& cpu) {
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 4;
}

void inst_ld_h_h(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.h, cpu.reg.h);
	cpu.clocks += 1;
}

void inst_ld_h_indiiy(ProcessorState& cpu) {
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 4;
}

void inst_ld_h_absbr(ProcessorState& cpu) {
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 3;
}

void inst_ld_inddix_h(ProcessorState& cpu) {
	auto addr0 = calc_indDIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.h);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_cp_sp_ba(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.sp, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_ld_h_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 2;
}

void inst_ld_inddiy_h(ProcessorState& cpu) {
	auto addr0 = calc_indDIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.h);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_cp_sp_hl(ProcessorState& cpu) {
	op_cp16(cpu, cpu.reg.sp, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_ld_h_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 2;
}

void inst_ld_indiix_h(ProcessorState& cpu) {
	auto addr0 = calc_indIIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.h);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_h_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 2;
}

void inst_ld_indiiy_h(ProcessorState& cpu) {
	auto addr0 = calc_indIIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.h);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_absix_a(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_abshl_inddix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_adc_ba_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_adc16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 4;
}

void inst_ld_absix_b(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.b);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_abshl_inddiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_adc_hl_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_adc16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 4;
}

void inst_ld_absix_l(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.l);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_abshl_indiix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_ba_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_sbc16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 4;
}

void inst_ld_absix_h(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.h);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_abshl_indiiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sbc_hl_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_sbc16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 4;
}

void inst_ld_absix_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_absix_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_absix_absix(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_absix_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_abshl_a(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_absix_inddix(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_add_sp_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_add16(cpu, cpu.reg.sp, data1);
	cpu.clocks += 4;
}

void inst_ld_abshl_b(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.b);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_absix_inddiy(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_abshl_l(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.l);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_absix_indiix(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_sub_sp_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_sub16(cpu, cpu.reg.sp, data1);
	cpu.clocks += 4;
}

void inst_ld_abshl_h(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.h);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_absix_indiiy(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_abshl_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_cp_sp_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_cp16(cpu, cpu.reg.sp, data1);
	cpu.clocks += 4;
}

void inst_ld_abshl_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_abshl_absix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_sp_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_ld16(cpu, cpu.reg.sp, data1);
	cpu.clocks += 4;
}

void inst_ld_abshl_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_absiy_a(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_ba_inddsp(ProcessorState& cpu) {
	auto addr1 = calc_indDSP(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 6;
}

void inst_ld_absiy_b(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.b);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_hl_inddsp(ProcessorState& cpu) {
	auto addr1 = calc_indDSP(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 6;
}

void inst_ld_absiy_l(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.l);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_ix_inddsp(ProcessorState& cpu) {
	auto addr1 = calc_indDSP(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ix, data1);
	cpu.clocks += 6;
}

void inst_ld_absiy_h(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.h);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 2;
}

void inst_ld_iy_inddsp(ProcessorState& cpu) {
	auto addr1 = calc_indDSP(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.iy, data1);
	cpu.clocks += 6;
}

void inst_ld_absiy_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	auto addr1 = calc_absBR(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_inddsp_ba(ProcessorState& cpu) {
	auto addr0 = calc_indDSP(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ba);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 6;
}

void inst_ld_absiy_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_inddsp_hl(ProcessorState& cpu) {
	auto addr0 = calc_indDSP(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.hl);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 6;
}

void inst_ld_absiy_absix(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_inddsp_ix(ProcessorState& cpu) {
	auto addr0 = calc_indDSP(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ix);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 6;
}

void inst_ld_absiy_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_inddsp_iy(ProcessorState& cpu) {
	auto addr0 = calc_indDSP(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.iy);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 6;
}

void inst_ld_absbr_a(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_absiy_inddix(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	auto addr1 = calc_indDIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_sp_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.sp, data1);
	cpu.clocks += 6;
}

void inst_ld_absbr_b(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.b);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_absiy_inddiy(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	auto addr1 = calc_indDIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_absbr_l(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.l);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_absiy_indiix(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	auto addr1 = calc_indIIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_absbr_h(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.h);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_ld_absiy_indiiy(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	auto addr1 = calc_indIIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_ind16_sp(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.sp);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 6;
}

void inst_ld_absbr_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0;
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_absbr_absix(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0;
	auto addr1 = calc_absIX(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_absbr_absiy(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0;
	auto addr1 = calc_absIY(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_inc_a(ProcessorState& cpu) {
	op_inc8(cpu, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_sla_a(ProcessorState& cpu) {
	op_sla8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_inc_b(ProcessorState& cpu) {
	op_inc8(cpu, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_sla_b(ProcessorState& cpu) {
	op_sla8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_inc_l(ProcessorState& cpu) {
	op_inc8(cpu, cpu.reg.l);
	cpu.clocks += 2;
}

void inst_sla_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_sla8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_inc_h(ProcessorState& cpu) {
	op_inc8(cpu, cpu.reg.h);
	cpu.clocks += 2;
}

void inst_sla_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_sla8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_inc_br(ProcessorState& cpu) {
	op_inc8(cpu, cpu.reg.br);
	cpu.clocks += 2;
}

void inst_sll_a(ProcessorState& cpu) {
	op_sll8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_inc_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_inc8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_sll_b(ProcessorState& cpu) {
	op_sll8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_inc_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_inc8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_sll_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_sll8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_inc_sp(ProcessorState& cpu) {
	op_inc16(cpu, cpu.reg.sp);
	cpu.clocks += 2;
}

void inst_sll_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_sll8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_dec_a(ProcessorState& cpu) {
	op_dec8(cpu, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_sra_a(ProcessorState& cpu) {
	op_sra8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_dec_b(ProcessorState& cpu) {
	op_dec8(cpu, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_sra_b(ProcessorState& cpu) {
	op_sra8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_dec_l(ProcessorState& cpu) {
	op_dec8(cpu, cpu.reg.l);
	cpu.clocks += 2;
}

void inst_sra_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_sra8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_dec_h(ProcessorState& cpu) {
	op_dec8(cpu, cpu.reg.h);
	cpu.clocks += 2;
}

void inst_sra_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_sra8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_dec_br(ProcessorState& cpu) {
	op_dec8(cpu, cpu.reg.br);
	cpu.clocks += 2;
}

void inst_srl_a(ProcessorState& cpu) {
	op_srl8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_dec_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_dec8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_srl_b(ProcessorState& cpu) {
	op_srl8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_dec_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_dec8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_srl_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_srl8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_dec_sp(ProcessorState& cpu) {
	op_dec16(cpu, cpu.reg.sp);
	cpu.clocks += 2;
}

void inst_srl_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_srl8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_inc_ba(ProcessorState& cpu) {
	op_inc16(cpu, cpu.reg.ba);
	cpu.clocks += 2;
}

void inst_rl_a(ProcessorState& cpu) {
	op_rl8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_inc_hl(ProcessorState& cpu) {
	op_inc16(cpu, cpu.reg.hl);
	cpu.clocks += 2;
}

void inst_rl_b(ProcessorState& cpu) {
	op_rl8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_inc_ix(ProcessorState& cpu) {
	op_inc16(cpu, cpu.reg.ix);
	cpu.clocks += 2;
}

void inst_rl_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_rl8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_inc_iy(ProcessorState& cpu) {
	op_inc16(cpu, cpu.reg.iy);
	cpu.clocks += 2;
}

void inst_rl_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_rl8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_bit_a_b(ProcessorState& cpu) {
	op_bit8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_rlc_a(ProcessorState& cpu) {
	op_rlc8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_bit_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_bit8(cpu, data0, data1);
	cpu.clocks += 3;
}

void inst_rlc_b(ProcessorState& cpu) {
	op_rlc8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_bit_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_bit8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_rlc_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_rlc8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_bit_b_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_bit8(cpu, cpu.reg.b, data1);
	cpu.clocks += 2;
}

void inst_rlc_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_rlc8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_dec_ba(ProcessorState& cpu) {
	op_dec16(cpu, cpu.reg.ba);
	cpu.clocks += 2;
}

void inst_rr_a(ProcessorState& cpu) {
	op_rr8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_dec_hl(ProcessorState& cpu) {
	op_dec16(cpu, cpu.reg.hl);
	cpu.clocks += 2;
}

void inst_rr_b(ProcessorState& cpu) {
	op_rr8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_dec_ix(ProcessorState& cpu) {
	op_dec16(cpu, cpu.reg.ix);
	cpu.clocks += 2;
}

void inst_rr_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_rr8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_dec_iy(ProcessorState& cpu) {
	op_dec16(cpu, cpu.reg.iy);
	cpu.clocks += 2;
}

void inst_rr_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_rr8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_and_sc_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_and8(cpu, cpu.reg.sc, data1);
	cpu.clocks += 3;
}

void inst_rrc_a(ProcessorState& cpu) {
	op_rrc8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_or_sc_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_or8(cpu, cpu.reg.sc, data1);
	cpu.clocks += 3;
}

void inst_rrc_b(ProcessorState& cpu) {
	op_rrc8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_xor_sc_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_xor8(cpu, cpu.reg.sc, data1);
	cpu.clocks += 3;
}

void inst_rrc_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_rrc8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_sc_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.sc, data1);
	cpu.clocks += 3;
}

void inst_rrc_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_rrc8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_push_ba(ProcessorState& cpu) {
	op_push16(cpu, cpu.reg.ba);
	cpu.clocks += 4;
}

void inst_cpl_a(ProcessorState& cpu) {
	op_cpl8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_push_hl(ProcessorState& cpu) {
	op_push16(cpu, cpu.reg.hl);
	cpu.clocks += 4;
}

void inst_cpl_b(ProcessorState& cpu) {
	op_cpl8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_push_ix(ProcessorState& cpu) {
	op_push16(cpu, cpu.reg.ix);
	cpu.clocks += 4;
}

void inst_cpl_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_cpl8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_push_iy(ProcessorState& cpu) {
	op_push16(cpu, cpu.reg.iy);
	cpu.clocks += 4;
}

void inst_cpl_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_cpl8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_push_br(ProcessorState& cpu) {
	op_push8(cpu, cpu.reg.br);
	cpu.clocks += 3;
}

void inst_neg_a(ProcessorState& cpu) {
	op_neg8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_push_ep(ProcessorState& cpu) {
	op_push8(cpu, cpu.reg.ep);
	cpu.clocks += 3;
}

void inst_neg_b(ProcessorState& cpu) {
	op_neg8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_neg_absbr(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_neg8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_push_sc(ProcessorState& cpu) {
	op_push8(cpu, cpu.reg.sc);
	cpu.clocks += 3;
}

void inst_neg_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_neg8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_pop_ba(ProcessorState& cpu) {
	op_pop16(cpu, cpu.reg.ba);
	cpu.clocks += 3;
}

void inst_pop_hl(ProcessorState& cpu) {
	op_pop16(cpu, cpu.reg.hl);
	cpu.clocks += 3;
}

void inst_pop_ix(ProcessorState& cpu) {
	op_pop16(cpu, cpu.reg.ix);
	cpu.clocks += 3;
}

void inst_pop_iy(ProcessorState& cpu) {
	op_pop16(cpu, cpu.reg.iy);
	cpu.clocks += 3;
}

void inst_pop_br(ProcessorState& cpu) {
	op_pop8(cpu, cpu.reg.br);
	cpu.clocks += 2;
}

void inst_pop_ep(ProcessorState& cpu) {
	op_pop8(cpu, cpu.reg.ep);
	cpu.clocks += 2;
}

void inst_pop_sc(ProcessorState& cpu) {
	op_pop8(cpu, cpu.reg.sc);
	cpu.clocks += 2;
}

void inst_ld_a_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 2;
}

void inst_and_b_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_and8(cpu, cpu.reg.b, data1);
	cpu.clocks += 3;
}

void inst_push_a(ProcessorState& cpu) {
	op_push8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_ld_b_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 2;
}

void inst_and_l_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_and8(cpu, cpu.reg.l, data1);
	cpu.clocks += 3;
}

void inst_push_b(ProcessorState& cpu) {
	op_push8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_ld_l_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 2;
}

void inst_and_h_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_and8(cpu, cpu.reg.h, data1);
	cpu.clocks += 3;
}

void inst_push_l(ProcessorState& cpu) {
	op_push8(cpu, cpu.reg.l);
	cpu.clocks += 3;
}

void inst_ld_h_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 2;
}

void inst_push_h(ProcessorState& cpu) {
	op_push8(cpu, cpu.reg.h);
	cpu.clocks += 3;
}

void inst_ld_br_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.br, data1);
	cpu.clocks += 2;
}

void inst_or_b_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_or8(cpu, cpu.reg.b, data1);
	cpu.clocks += 3;
}

void inst_pop_a(ProcessorState& cpu) {
	op_pop8(cpu, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_ld_abshl_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0;
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_or_l_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_or8(cpu, cpu.reg.l, data1);
	cpu.clocks += 3;
}

void inst_pop_b(ProcessorState& cpu) {
	op_pop8(cpu, cpu.reg.b);
	cpu.clocks += 3;
}

void inst_ld_absix_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint8_t data0;
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_or_h_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_or8(cpu, cpu.reg.h, data1);
	cpu.clocks += 3;
}

void inst_pop_l(ProcessorState& cpu) {
	op_pop8(cpu, cpu.reg.l);
	cpu.clocks += 3;
}

void inst_ld_absiy_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint8_t data0;
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_pop_h(ProcessorState& cpu) {
	op_pop8(cpu, cpu.reg.h);
	cpu.clocks += 3;
}

void inst_ld_ba_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 5;
}

void inst_xor_b_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_xor8(cpu, cpu.reg.b, data1);
	cpu.clocks += 3;
}

void inst_ld_hl_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 5;
}

void inst_xor_l_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_xor8(cpu, cpu.reg.l, data1);
	cpu.clocks += 3;
}

void inst_ld_ix_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ix, data1);
	cpu.clocks += 5;
}

void inst_xor_h_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_xor8(cpu, cpu.reg.h, data1);
	cpu.clocks += 3;
}

void inst_ld_iy_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.iy, data1);
	cpu.clocks += 5;
}

void inst_ld_ind16_ba(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ba);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_cp_b_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_cp8(cpu, cpu.reg.b, data1);
	cpu.clocks += 3;
}

void inst_ld_ind16_hl(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.hl);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_cp_l_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_cp8(cpu, cpu.reg.l, data1);
	cpu.clocks += 3;
}

void inst_ld_ind16_ix(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ix);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_cp_h_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_cp8(cpu, cpu.reg.h, data1);
	cpu.clocks += 3;
}

void inst_ld_ind16_iy(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.iy);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_cp_br_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_cp8(cpu, cpu.reg.br, data1);
	cpu.clocks += 3;
}

void inst_add_ba_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_add16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 3;
}

void inst_ld_a_br(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.br);
	cpu.clocks += 2;
}

void inst_ld_ba_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 5;
}

void inst_add_hl_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_add16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 3;
}

void inst_ld_a_sc(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.sc);
	cpu.clocks += 2;
}

void inst_ld_hl_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 5;
}

void inst_add_ix_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_add16(cpu, cpu.reg.ix, data1);
	cpu.clocks += 3;
}

void inst_ld_br_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.br, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_ld_ix_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ix, data1);
	cpu.clocks += 5;
}

void inst_add_iy_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_add16(cpu, cpu.reg.iy, data1);
	cpu.clocks += 3;
}

void inst_ld_sc_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.sc, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_ld_iy_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.iy, data1);
	cpu.clocks += 5;
}

void inst_ld_ba_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_ld16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 3;
}

void inst_ld_nb_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.nb, data1);
	cpu.clocks += 4;
}

void inst_ld_abshl_ba(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ba);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_hl_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_ld16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 3;
}

void inst_ld_ep_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.ep, data1);
	cpu.clocks += 3;
}

void inst_ld_abshl_hl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.hl);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_ix_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_ld16(cpu, cpu.reg.ix, data1);
	cpu.clocks += 3;
}

void inst_ld_xp_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.xp, data1);
	cpu.clocks += 3;
}

void inst_ld_abshl_ix(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ix);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_iy_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_ld16(cpu, cpu.reg.iy, data1);
	cpu.clocks += 3;
}

void inst_ld_yp_imm8(ProcessorState& cpu) {
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, cpu.reg.yp, data1);
	cpu.clocks += 3;
}

void inst_ld_abshl_iy(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.iy);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ex_ba_hl(ProcessorState& cpu) {
	op_ex16(cpu, cpu.reg.ba, cpu.reg.hl);
	cpu.clocks += 3;
}

void inst_ld_a_nb(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.nb);
	cpu.clocks += 2;
}

void inst_ex_ba_ix(ProcessorState& cpu) {
	op_ex16(cpu, cpu.reg.ba, cpu.reg.ix);
	cpu.clocks += 3;
}

void inst_ld_a_ep(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.ep);
	cpu.clocks += 2;
}

void inst_ex_ba_iy(ProcessorState& cpu) {
	op_ex16(cpu, cpu.reg.ba, cpu.reg.iy);
	cpu.clocks += 3;
}

void inst_ld_a_xp(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.xp);
	cpu.clocks += 2;
}

void inst_ex_ba_sp(ProcessorState& cpu) {
	op_ex16(cpu, cpu.reg.ba, cpu.reg.sp);
	cpu.clocks += 3;
}

void inst_ld_a_yp(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.a, cpu.reg.yp);
	cpu.clocks += 2;
}

void inst_ex_a_b(ProcessorState& cpu) {
	op_ex8(cpu, cpu.reg.a, cpu.reg.b);
	cpu.clocks += 2;
}

void inst_ld_nb_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.nb, cpu.reg.a);
	cpu.clocks += 3;
}

void inst_ex_a_abshl(ProcessorState& cpu) {
	auto addr1 = calc_absHL(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ex8(cpu, cpu.reg.a, data1);
	cpu_write8(cpu, data1, addr1);
	cpu.clocks += 3;
}

void inst_ld_ep_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.ep, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_ld_xp_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.xp, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_ld_yp_a(ProcessorState& cpu) {
	op_ld8(cpu, cpu.reg.yp, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_sub_ba_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_sub16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 3;
}

void inst_ld_a_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.a, data1);
	cpu.clocks += 5;
}

void inst_ld_ba_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 5;
}

void inst_sub_hl_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_sub16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 3;
}

void inst_ld_b_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.b, data1);
	cpu.clocks += 5;
}

void inst_ld_hl_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 5;
}

void inst_sub_ix_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_sub16(cpu, cpu.reg.ix, data1);
	cpu.clocks += 3;
}

void inst_ld_l_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.l, data1);
	cpu.clocks += 5;
}

void inst_ld_ix_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ix, data1);
	cpu.clocks += 5;
}

void inst_sub_iy_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_sub16(cpu, cpu.reg.iy, data1);
	cpu.clocks += 3;
}

void inst_ld_h_ind16(ProcessorState& cpu) {
	auto addr1 = calc_ind16(cpu);
	uint8_t data1 = cpu_read8(cpu, addr1);
	op_ld8(cpu, cpu.reg.h, data1);
	cpu.clocks += 5;
}

void inst_ld_iy_absix(ProcessorState& cpu) {
	auto addr1 = calc_absIX(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.iy, data1);
	cpu.clocks += 5;
}

void inst_cp_ba_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_cp16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 3;
}

void inst_ld_ind16_a(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.a);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_absix_ba(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ba);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_cp_hl_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_cp16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 3;
}

void inst_ld_ind16_b(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.b);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_absix_hl(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.hl);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_cp_ix_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_cp16(cpu, cpu.reg.ix, data1);
	cpu.clocks += 3;
}

void inst_ld_ind16_l(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.l);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_absix_ix(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ix);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_cp_iy_imm16(ProcessorState& cpu) {
	uint16_t data1 = cpu_imm16(cpu);
	op_cp16(cpu, cpu.reg.iy, data1);
	cpu.clocks += 3;
}

void inst_ld_ind16_h(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint8_t data0;
	op_ld8(cpu, data0, cpu.reg.h);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_absix_iy(ProcessorState& cpu) {
	auto addr0 = calc_absIX(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.iy);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_and_absbr_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_and8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_ba_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ba, data1);
	cpu.clocks += 5;
}

void inst_or_absbr_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_or8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_hl_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.hl, data1);
	cpu.clocks += 5;
}

void inst_xor_absbr_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_xor8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_ix_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.ix, data1);
	cpu.clocks += 5;
}

void inst_cp_absbr_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_cp8(cpu, data0, data1);
	cpu.clocks += 4;
}

void inst_ld_iy_absiy(ProcessorState& cpu) {
	auto addr1 = calc_absIY(cpu);
	uint16_t data1 = cpu_read16(cpu, addr1);
	op_ld16(cpu, cpu.reg.iy, data1);
	cpu.clocks += 5;
}

void inst_bit_absbr_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	uint8_t data1 = cpu_imm8(cpu);
	op_bit8(cpu, data0, data1);
	cpu.clocks += 4;
}

void inst_ld_absiy_ba(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ba);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_absbr_imm8(ProcessorState& cpu) {
	auto addr0 = calc_absBR(cpu);
	uint8_t data0;
	uint8_t data1 = cpu_imm8(cpu);
	op_ld8(cpu, data0, data1);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 4;
}

void inst_ld_absiy_hl(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.hl);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_absiy_ix(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.ix);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_ld_absiy_iy(ProcessorState& cpu) {
	auto addr0 = calc_absIY(cpu);
	uint16_t data0;
	op_ld16(cpu, data0, cpu.reg.iy);
	cpu_write16(cpu, data0, addr0);
	cpu.clocks += 5;
}

void inst_cars_c_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.c)) {
		cpu.clocks += 2;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 5;
}

void inst_jrs_lt_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.n != cpu.reg.flag.v)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_ba_ba(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ba, cpu.reg.ba);
	cpu.clocks += 2;
}

void inst_cars_nc_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.c)) {
		cpu.clocks += 2;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 5;
}

void inst_jrs_le_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!((cpu.reg.flag.n != cpu.reg.flag.v) || cpu.reg.flag.z)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_ba_hl(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ba, cpu.reg.hl);
	cpu.clocks += 2;
}

void inst_cars_z_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.z)) {
		cpu.clocks += 2;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 5;
}

void inst_jrs_gt_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!((cpu.reg.flag.n == cpu.reg.flag.v) && !cpu.reg.flag.z)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_ba_ix(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ba, cpu.reg.ix);
	cpu.clocks += 2;
}

void inst_cars_nz_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.z)) {
		cpu.clocks += 2;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 5;
}

void inst_jrs_ge_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.n == cpu.reg.flag.v)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_ba_iy(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ba, cpu.reg.iy);
	cpu.clocks += 2;
}

void inst_jrs_c_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.c)) {
		cpu.clocks += 2;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 2;
}

void inst_jrs_v_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.v)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_hl_ba(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.hl, cpu.reg.ba);
	cpu.clocks += 2;
}

void inst_jrs_nc_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.c)) {
		cpu.clocks += 2;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 2;
}

void inst_jrs_nv_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.v)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_hl_hl(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.hl, cpu.reg.hl);
	cpu.clocks += 2;
}

void inst_jrs_z_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.z)) {
		cpu.clocks += 2;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 2;
}

void inst_jrs_p_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.n)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_hl_ix(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.hl, cpu.reg.ix);
	cpu.clocks += 2;
}

void inst_jrs_nz_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.z)) {
		cpu.clocks += 2;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 2;
}

void inst_jrs_m_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.n)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_hl_iy(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.hl, cpu.reg.iy);
	cpu.clocks += 2;
}

void inst_carl_c_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	if (!(cpu.reg.flag.c)) {
		cpu.clocks += 3;
		return ;
	}
	op_carl16(cpu, data0);
	cpu.clocks += 6;
}

void inst_jrs_f0_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.f0)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_ix_ba(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ix, cpu.reg.ba);
	cpu.clocks += 2;
}

void inst_carl_nc_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	if (!(!cpu.reg.flag.c)) {
		cpu.clocks += 3;
		return ;
	}
	op_carl16(cpu, data0);
	cpu.clocks += 6;
}

void inst_jrs_f1_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.f1)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_ix_hl(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ix, cpu.reg.hl);
	cpu.clocks += 2;
}

void inst_carl_z_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	if (!(!cpu.reg.flag.z)) {
		cpu.clocks += 3;
		return ;
	}
	op_carl16(cpu, data0);
	cpu.clocks += 6;
}

void inst_jrs_f2_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.f2)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_ix_ix(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ix, cpu.reg.ix);
	cpu.clocks += 2;
}

void inst_carl_nz_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	if (!(!cpu.reg.flag.z)) {
		cpu.clocks += 3;
		return ;
	}
	op_carl16(cpu, data0);
	cpu.clocks += 6;
}

void inst_jrs_f3_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.f3)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_ix_iy(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ix, cpu.reg.iy);
	cpu.clocks += 2;
}

void inst_jrl_c_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	if (!(cpu.reg.flag.c)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrl16(cpu, data0);
	cpu.clocks += 3;
}

void inst_jrs_nf0_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.f0)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_iy_ba(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.iy, cpu.reg.ba);
	cpu.clocks += 2;
}

void inst_jrl_nc_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	if (!(!cpu.reg.flag.c)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrl16(cpu, data0);
	cpu.clocks += 3;
}

void inst_jrs_nf1_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.f1)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_iy_hl(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.iy, cpu.reg.hl);
	cpu.clocks += 2;
}

void inst_jrl_z_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	if (!(!cpu.reg.flag.z)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrl16(cpu, data0);
	cpu.clocks += 3;
}

void inst_jrs_nf2_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.f2)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_iy_ix(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.iy, cpu.reg.ix);
	cpu.clocks += 2;
}

void inst_jrl_nz_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	if (!(!cpu.reg.flag.z)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrl16(cpu, data0);
	cpu.clocks += 3;
}

void inst_jrs_nf3_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.f3)) {
		cpu.clocks += 3;
		return ;
	}
	op_jrs8(cpu, data0);
	cpu.clocks += 3;
}

void inst_ld_iy_iy(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.iy, cpu.reg.iy);
	cpu.clocks += 2;
}

void inst_cars_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	op_cars8(cpu, data0);
	cpu.clocks += 5;
}

void inst_cars_lt_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.n != cpu.reg.flag.v)) {
		cpu.clocks += 3;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_sp_ba(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.sp, cpu.reg.ba);
	cpu.clocks += 2;
}

void inst_jrs_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	op_jrs8(cpu, data0);
	cpu.clocks += 2;
}

void inst_cars_le_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!((cpu.reg.flag.n != cpu.reg.flag.v) || cpu.reg.flag.z)) {
		cpu.clocks += 4;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_sp_hl(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.sp, cpu.reg.hl);
	cpu.clocks += 2;
}

void inst_carl_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	op_carl16(cpu, data0);
	cpu.clocks += 6;
}

void inst_cars_gt_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!((cpu.reg.flag.n == cpu.reg.flag.v) && !cpu.reg.flag.z)) {
		cpu.clocks += 5;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_sp_ix(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.sp, cpu.reg.ix);
	cpu.clocks += 2;
}

void inst_jrl_imm16(ProcessorState& cpu) {
	uint16_t data0 = cpu_imm16(cpu);
	op_jrl16(cpu, data0);
	cpu.clocks += 3;
}

void inst_cars_ge_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.n == cpu.reg.flag.v)) {
		cpu.clocks += 6;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_sp_iy(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.sp, cpu.reg.iy);
	cpu.clocks += 2;
}

void inst_jp_hl(ProcessorState& cpu) {
	op_jp16(cpu, cpu.reg.hl);
	cpu.clocks += 2;
}

void inst_cars_v_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.v)) {
		cpu.clocks += 7;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_hl_sp(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.hl, cpu.reg.sp);
	cpu.clocks += 2;
}

void inst_djr_nz_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.z)) {
		cpu.clocks += 4;
		return ;
	}
	op_djr8(cpu, data0);
	cpu.clocks += 4;
}

void inst_cars_nv_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.v)) {
		cpu.clocks += 8;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_hl_pc(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.hl, cpu.reg.pc);
	cpu.clocks += 2;
}

void inst_swap_a(ProcessorState& cpu) {
	op_swap8(cpu, cpu.reg.a);
	cpu.clocks += 2;
}

void inst_cars_p_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.n)) {
		cpu.clocks += 9;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_swap_abshl(ProcessorState& cpu) {
	auto addr0 = calc_absHL(cpu);
	uint8_t data0 = cpu_read8(cpu, addr0);
	op_swap8(cpu, data0);
	cpu_write8(cpu, data0, addr0);
	cpu.clocks += 3;
}

void inst_cars_m_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.n)) {
		cpu.clocks += 10;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_cars_f0_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.f0)) {
		cpu.clocks += 11;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_ba_sp(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ba, cpu.reg.sp);
	cpu.clocks += 2;
}

void inst_cars_f1_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.f1)) {
		cpu.clocks += 12;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_ba_pc(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ba, cpu.reg.pc);
	cpu.clocks += 2;
}

void inst_cars_f2_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.f2)) {
		cpu.clocks += 13;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_ix_sp(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.ix, cpu.reg.sp);
	cpu.clocks += 2;
}

void inst_call_ind16(ProcessorState& cpu) {
	auto addr0 = calc_ind16(cpu);
	uint16_t data0 = cpu_read16(cpu, addr0);
	op_call16(cpu, data0);
	cpu.clocks += 8;
}

void inst_cars_f3_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(cpu.reg.flag.f3)) {
		cpu.clocks += 14;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_int_vect(ProcessorState& cpu) {
	auto addr0 = calc_vect(cpu);
	uint16_t data0 = cpu_read16(cpu, addr0);
	op_int16(cpu, data0);
	cpu.clocks += 8;
}

void inst_cars_nf0_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.f0)) {
		cpu.clocks += 15;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_jp_vect(ProcessorState& cpu) {
	auto addr0 = calc_vect(cpu);
	uint16_t data0 = cpu_read16(cpu, addr0);
	op_jp16(cpu, data0);
	cpu.clocks += 4;
}

void inst_cars_nf1_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.f1)) {
		cpu.clocks += 16;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_cars_nf2_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.f2)) {
		cpu.clocks += 17;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

void inst_ld_iy_sp(ProcessorState& cpu) {
	op_ld16(cpu, cpu.reg.iy, cpu.reg.sp);
	cpu.clocks += 2;
}

void inst_cars_nf3_imm8(ProcessorState& cpu) {
	uint8_t data0 = cpu_imm8(cpu);
	if (!(!cpu.reg.flag.f3)) {
		cpu.clocks += 18;
		return ;
	}
	op_cars8(cpu, data0);
	cpu.clocks += 6;
}

static InstructionCall inst_table0[] = {
	inst_add_a_a,
	inst_add_a_b,
	inst_add_a_imm8,
	inst_add_a_abshl,
	inst_add_a_absbr,
	inst_add_a_ind16,
	inst_add_a_absix,
	inst_add_a_absiy,
	inst_adc_a_a,
	inst_adc_a_b,
	inst_adc_a_imm8,
	inst_adc_a_abshl,
	inst_adc_a_absbr,
	inst_adc_a_ind16,
	inst_adc_a_absix,
	inst_adc_a_absiy,
	inst_sub_a_a,
	inst_sub_a_b,
	inst_sub_a_imm8,
	inst_sub_a_abshl,
	inst_sub_a_absbr,
	inst_sub_a_ind16,
	inst_sub_a_absix,
	inst_sub_a_absiy,
	inst_sbc_a_a,
	inst_sbc_a_b,
	inst_sbc_a_imm8,
	inst_sbc_a_abshl,
	inst_sbc_a_absbr,
	inst_sbc_a_ind16,
	inst_sbc_a_absix,
	inst_sbc_a_absiy,
	inst_and_a_a,
	inst_and_a_b,
	inst_and_a_imm8,
	inst_and_a_abshl,
	inst_and_a_absbr,
	inst_and_a_ind16,
	inst_and_a_absix,
	inst_and_a_absiy,
	inst_or_a_a,
	inst_or_a_b,
	inst_or_a_imm8,
	inst_or_a_abshl,
	inst_or_a_absbr,
	inst_or_a_ind16,
	inst_or_a_absix,
	inst_or_a_absiy,
	inst_cp_a_a,
	inst_cp_a_b,
	inst_cp_a_imm8,
	inst_cp_a_abshl,
	inst_cp_a_absbr,
	inst_cp_a_ind16,
	inst_cp_a_absix,
	inst_cp_a_absiy,
	inst_xor_a_a,
	inst_xor_a_b,
	inst_xor_a_imm8,
	inst_xor_a_abshl,
	inst_xor_a_absbr,
	inst_xor_a_ind16,
	inst_xor_a_absix,
	inst_xor_a_absiy,
	inst_ld_a_a,
	inst_ld_a_b,
	inst_ld_a_l,
	inst_ld_a_h,
	inst_ld_a_absbr,
	inst_ld_a_abshl,
	inst_ld_a_absix,
	inst_ld_a_absiy,
	inst_ld_b_a,
	inst_ld_b_b,
	inst_ld_b_l,
	inst_ld_b_h,
	inst_ld_b_absbr,
	inst_ld_b_abshl,
	inst_ld_b_absix,
	inst_ld_b_absiy,
	inst_ld_l_a,
	inst_ld_l_b,
	inst_ld_l_l,
	inst_ld_l_h,
	inst_ld_l_absbr,
	inst_ld_l_abshl,
	inst_ld_l_absix,
	inst_ld_l_absiy,
	inst_ld_h_a,
	inst_ld_h_b,
	inst_ld_h_l,
	inst_ld_h_h,
	inst_ld_h_absbr,
	inst_ld_h_abshl,
	inst_ld_h_absix,
	inst_ld_h_absiy,
	inst_ld_absix_a,
	inst_ld_absix_b,
	inst_ld_absix_l,
	inst_ld_absix_h,
	inst_ld_absix_absbr,
	inst_ld_absix_abshl,
	inst_ld_absix_absix,
	inst_ld_absix_absiy,
	inst_ld_abshl_a,
	inst_ld_abshl_b,
	inst_ld_abshl_l,
	inst_ld_abshl_h,
	inst_ld_abshl_absbr,
	inst_ld_abshl_abshl,
	inst_ld_abshl_absix,
	inst_ld_abshl_absiy,
	inst_ld_absiy_a,
	inst_ld_absiy_b,
	inst_ld_absiy_l,
	inst_ld_absiy_h,
	inst_ld_absiy_absbr,
	inst_ld_absiy_abshl,
	inst_ld_absiy_absix,
	inst_ld_absiy_absiy,
	inst_ld_absbr_a,
	inst_ld_absbr_b,
	inst_ld_absbr_l,
	inst_ld_absbr_h,
	inst_undefined,
	inst_ld_absbr_abshl,
	inst_ld_absbr_absix,
	inst_ld_absbr_absiy,
	inst_inc_a,
	inst_inc_b,
	inst_inc_l,
	inst_inc_h,
	inst_inc_br,
	inst_inc_absbr,
	inst_inc_abshl,
	inst_inc_sp,
	inst_dec_a,
	inst_dec_b,
	inst_dec_l,
	inst_dec_h,
	inst_dec_br,
	inst_dec_absbr,
	inst_dec_abshl,
	inst_dec_sp,
	inst_inc_ba,
	inst_inc_hl,
	inst_inc_ix,
	inst_inc_iy,
	inst_bit_a_b,
	inst_bit_abshl_imm8,
	inst_bit_a_imm8,
	inst_bit_b_imm8,
	inst_dec_ba,
	inst_dec_hl,
	inst_dec_ix,
	inst_dec_iy,
	inst_and_sc_imm8,
	inst_or_sc_imm8,
	inst_xor_sc_imm8,
	inst_ld_sc_imm8,
	inst_push_ba,
	inst_push_hl,
	inst_push_ix,
	inst_push_iy,
	inst_push_br,
	inst_push_ep,
	inst_push_ip,
	inst_push_sc,
	inst_pop_ba,
	inst_pop_hl,
	inst_pop_ix,
	inst_pop_iy,
	inst_pop_br,
	inst_pop_ep,
	inst_pop_ip,
	inst_pop_sc,
	inst_ld_a_imm8,
	inst_ld_b_imm8,
	inst_ld_l_imm8,
	inst_ld_h_imm8,
	inst_ld_br_imm8,
	inst_ld_abshl_imm8,
	inst_ld_absix_imm8,
	inst_ld_absiy_imm8,
	inst_ld_ba_ind16,
	inst_ld_hl_ind16,
	inst_ld_ix_ind16,
	inst_ld_iy_ind16,
	inst_ld_ind16_ba,
	inst_ld_ind16_hl,
	inst_ld_ind16_ix,
	inst_ld_ind16_iy,
	inst_add_ba_imm16,
	inst_add_hl_imm16,
	inst_add_ix_imm16,
	inst_add_iy_imm16,
	inst_ld_ba_imm16,
	inst_ld_hl_imm16,
	inst_ld_ix_imm16,
	inst_ld_iy_imm16,
	inst_ex_ba_hl,
	inst_ex_ba_ix,
	inst_ex_ba_iy,
	inst_ex_ba_sp,
	inst_ex_a_b,
	inst_ex_a_abshl,
	inst_extended_ce,
	inst_extended_cf,
	inst_sub_ba_imm16,
	inst_sub_hl_imm16,
	inst_sub_ix_imm16,
	inst_sub_iy_imm16,
	inst_cp_ba_imm16,
	inst_cp_hl_imm16,
	inst_cp_ix_imm16,
	inst_cp_iy_imm16,
	inst_and_absbr_imm8,
	inst_or_absbr_imm8,
	inst_xor_absbr_imm8,
	inst_cp_absbr_imm8,
	inst_bit_absbr_imm8,
	inst_ld_absbr_imm8,
	inst_pack,
	inst_upck,
	inst_cars_c_imm8,
	inst_cars_nc_imm8,
	inst_cars_z_imm8,
	inst_cars_nz_imm8,
	inst_jrs_c_imm8,
	inst_jrs_nc_imm8,
	inst_jrs_z_imm8,
	inst_jrs_nz_imm8,
	inst_carl_c_imm16,
	inst_carl_nc_imm16,
	inst_carl_z_imm16,
	inst_carl_nz_imm16,
	inst_jrl_c_imm16,
	inst_jrl_nc_imm16,
	inst_jrl_z_imm16,
	inst_jrl_nz_imm16,
	inst_cars_imm8,
	inst_jrs_imm8,
	inst_carl_imm16,
	inst_jrl_imm16,
	inst_jp_hl,
	inst_djr_nz_imm8,
	inst_swap_a,
	inst_swap_abshl,
	inst_ret,
	inst_rete,
	inst_rets,
	inst_call_ind16,
	inst_int_vect,
	inst_jp_vect,
	inst_undefined,
	inst_nop,
};
static InstructionCall inst_table1[] = {
	inst_add_a_inddix,
	inst_add_a_inddiy,
	inst_add_a_indiix,
	inst_add_a_indiiy,
	inst_add_abshl_a,
	inst_add_abshl_imm8,
	inst_add_abshl_absix,
	inst_add_abshl_absiy,
	inst_adc_a_inddix,
	inst_adc_a_inddiy,
	inst_adc_a_indiix,
	inst_adc_a_indiiy,
	inst_adc_abshl_a,
	inst_adc_abshl_imm8,
	inst_adc_abshl_absix,
	inst_adc_abshl_absiy,
	inst_sub_a_inddix,
	inst_sub_a_inddiy,
	inst_sub_a_indiix,
	inst_sub_a_indiiy,
	inst_sub_abshl_a,
	inst_sub_abshl_imm8,
	inst_sub_abshl_absix,
	inst_sub_abshl_absiy,
	inst_sbc_a_inddix,
	inst_sbc_a_inddiy,
	inst_sbc_a_indiix,
	inst_sbc_a_indiiy,
	inst_sbc_abshl_a,
	inst_sbc_abshl_imm8,
	inst_sbc_abshl_absix,
	inst_sbc_abshl_absiy,
	inst_and_a_inddix,
	inst_and_a_inddiy,
	inst_and_a_indiix,
	inst_and_a_indiiy,
	inst_and_abshl_a,
	inst_and_abshl_imm8,
	inst_and_abshl_absix,
	inst_and_abshl_absiy,
	inst_or_a_inddix,
	inst_or_a_inddiy,
	inst_or_a_indiix,
	inst_or_a_indiiy,
	inst_or_abshl_a,
	inst_or_abshl_imm8,
	inst_or_abshl_absix,
	inst_or_abshl_absiy,
	inst_cp_a_inddix,
	inst_cp_a_inddiy,
	inst_cp_a_indiix,
	inst_cp_a_indiiy,
	inst_cp_abshl_a,
	inst_cp_abshl_imm8,
	inst_cp_abshl_absix,
	inst_cp_abshl_absiy,
	inst_xor_a_inddix,
	inst_xor_a_inddiy,
	inst_xor_a_indiix,
	inst_xor_a_indiiy,
	inst_xor_abshl_a,
	inst_xor_abshl_imm8,
	inst_xor_abshl_absix,
	inst_xor_abshl_absiy,
	inst_ld_a_inddix,
	inst_ld_a_inddiy,
	inst_ld_a_indiix,
	inst_ld_a_indiiy,
	inst_ld_inddix_a,
	inst_ld_inddiy_a,
	inst_ld_indiix_a,
	inst_ld_indiiy_a,
	inst_ld_b_inddix,
	inst_ld_b_inddiy,
	inst_ld_b_indiix,
	inst_ld_b_indiiy,
	inst_ld_inddix_b,
	inst_ld_inddiy_b,
	inst_ld_indiix_b,
	inst_ld_indiiy_b,
	inst_ld_l_inddix,
	inst_ld_l_inddiy,
	inst_ld_l_indiix,
	inst_ld_l_indiiy,
	inst_ld_inddix_l,
	inst_ld_inddiy_l,
	inst_ld_indiix_l,
	inst_ld_indiiy_l,
	inst_ld_h_inddix,
	inst_ld_h_inddiy,
	inst_ld_h_indiix,
	inst_ld_h_indiiy,
	inst_ld_inddix_h,
	inst_ld_inddiy_h,
	inst_ld_indiix_h,
	inst_ld_indiiy_h,
	inst_ld_abshl_inddix,
	inst_ld_abshl_inddiy,
	inst_ld_abshl_indiix,
	inst_ld_abshl_indiiy,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_ld_absix_inddix,
	inst_ld_absix_inddiy,
	inst_ld_absix_indiix,
	inst_ld_absix_indiiy,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_ld_absiy_inddix,
	inst_ld_absiy_inddiy,
	inst_ld_absiy_indiix,
	inst_ld_absiy_indiiy,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_sla_a,
	inst_sla_b,
	inst_sla_absbr,
	inst_sla_abshl,
	inst_sll_a,
	inst_sll_b,
	inst_sll_absbr,
	inst_sll_abshl,
	inst_sra_a,
	inst_sra_b,
	inst_sra_absbr,
	inst_sra_abshl,
	inst_srl_a,
	inst_srl_b,
	inst_srl_absbr,
	inst_srl_abshl,
	inst_rl_a,
	inst_rl_b,
	inst_rl_absbr,
	inst_rl_abshl,
	inst_rlc_a,
	inst_rlc_b,
	inst_rlc_absbr,
	inst_rlc_abshl,
	inst_rr_a,
	inst_rr_b,
	inst_rr_absbr,
	inst_rr_abshl,
	inst_rrc_a,
	inst_rrc_b,
	inst_rrc_absbr,
	inst_rrc_abshl,
	inst_cpl_a,
	inst_cpl_b,
	inst_cpl_absbr,
	inst_cpl_abshl,
	inst_neg_a,
	inst_neg_b,
	inst_neg_absbr,
	inst_neg_abshl,
	inst_sep,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_halt,
	inst_slp,
	inst_and_b_imm8,
	inst_and_l_imm8,
	inst_and_h_imm8,
	inst_undefined,
	inst_or_b_imm8,
	inst_or_l_imm8,
	inst_or_h_imm8,
	inst_undefined,
	inst_xor_b_imm8,
	inst_xor_l_imm8,
	inst_xor_h_imm8,
	inst_undefined,
	inst_cp_b_imm8,
	inst_cp_l_imm8,
	inst_cp_h_imm8,
	inst_cp_br_imm8,
	inst_ld_a_br,
	inst_ld_a_sc,
	inst_ld_br_a,
	inst_ld_sc_a,
	inst_ld_nb_imm8,
	inst_ld_ep_imm8,
	inst_ld_xp_imm8,
	inst_ld_yp_imm8,
	inst_ld_a_nb,
	inst_ld_a_ep,
	inst_ld_a_xp,
	inst_ld_a_yp,
	inst_ld_nb_a,
	inst_ld_ep_a,
	inst_ld_xp_a,
	inst_ld_yp_a,
	inst_ld_a_ind16,
	inst_ld_b_ind16,
	inst_ld_l_ind16,
	inst_ld_h_ind16,
	inst_ld_ind16_a,
	inst_ld_ind16_b,
	inst_ld_ind16_l,
	inst_ld_ind16_h,
	inst_mul,
	inst_div,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_jrs_lt_imm8,
	inst_jrs_le_imm8,
	inst_jrs_gt_imm8,
	inst_jrs_ge_imm8,
	inst_jrs_v_imm8,
	inst_jrs_nv_imm8,
	inst_jrs_p_imm8,
	inst_jrs_m_imm8,
	inst_jrs_f0_imm8,
	inst_jrs_f1_imm8,
	inst_jrs_f2_imm8,
	inst_jrs_f3_imm8,
	inst_jrs_nf0_imm8,
	inst_jrs_nf1_imm8,
	inst_jrs_nf2_imm8,
	inst_jrs_nf3_imm8,
	inst_cars_lt_imm8,
	inst_cars_le_imm8,
	inst_cars_gt_imm8,
	inst_cars_ge_imm8,
	inst_cars_v_imm8,
	inst_cars_nv_imm8,
	inst_cars_p_imm8,
	inst_cars_m_imm8,
	inst_cars_f0_imm8,
	inst_cars_f1_imm8,
	inst_cars_f2_imm8,
	inst_cars_f3_imm8,
	inst_cars_nf0_imm8,
	inst_cars_nf1_imm8,
	inst_cars_nf2_imm8,
	inst_cars_nf3_imm8,
};
static InstructionCall inst_table2[] = {
	inst_add_ba_ba,
	inst_add_ba_hl,
	inst_add_ba_ix,
	inst_add_ba_iy,
	inst_adc_ba_ba,
	inst_adc_ba_hl,
	inst_adc_ba_ix,
	inst_adc_ba_iy,
	inst_sub_ba_ba,
	inst_sub_ba_hl,
	inst_sub_ba_ix,
	inst_sub_ba_iy,
	inst_sbc_ba_ba,
	inst_sbc_ba_hl,
	inst_sbc_ba_ix,
	inst_sbc_ba_iy,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_cp_ba_ba,
	inst_cp_ba_hl,
	inst_cp_ba_ix,
	inst_cp_ba_iy,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_add_hl_ba,
	inst_add_hl_hl,
	inst_add_hl_ix,
	inst_add_hl_iy,
	inst_adc_hl_ba,
	inst_adc_hl_hl,
	inst_adc_hl_ix,
	inst_adc_hl_iy,
	inst_sub_hl_ba,
	inst_sub_hl_hl,
	inst_sub_hl_ix,
	inst_sub_hl_iy,
	inst_sbc_hl_ba,
	inst_sbc_hl_hl,
	inst_sbc_hl_ix,
	inst_sbc_hl_iy,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_cp_hl_ba,
	inst_cp_hl_hl,
	inst_cp_hl_ix,
	inst_cp_hl_iy,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_add_ix_ba,
	inst_add_ix_hl,
	inst_add_iy_ba,
	inst_add_iy_hl,
	inst_add_sp_ba,
	inst_add_sp_hl,
	inst_undefined,
	inst_undefined,
	inst_sub_ix_ba,
	inst_sub_ix_hl,
	inst_sub_iy_ba,
	inst_sub_iy_hl,
	inst_sub_sp_ba,
	inst_sub_sp_hl,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_cp_sp_ba,
	inst_cp_sp_hl,
	inst_undefined,
	inst_undefined,
	inst_adc_ba_imm16,
	inst_adc_hl_imm16,
	inst_sbc_ba_imm16,
	inst_sbc_hl_imm16,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_add_sp_imm16,
	inst_undefined,
	inst_sub_sp_imm16,
	inst_undefined,
	inst_cp_sp_imm16,
	inst_undefined,
	inst_ld_sp_imm16,
	inst_undefined,
	inst_ld_ba_inddsp,
	inst_ld_hl_inddsp,
	inst_ld_ix_inddsp,
	inst_ld_iy_inddsp,
	inst_ld_inddsp_ba,
	inst_ld_inddsp_hl,
	inst_ld_inddsp_ix,
	inst_ld_inddsp_iy,
	inst_ld_sp_ind16,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_ld_ind16_sp,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_push_a,
	inst_push_b,
	inst_push_l,
	inst_push_h,
	inst_pop_a,
	inst_pop_b,
	inst_pop_l,
	inst_pop_h,
	inst_push_all,
	inst_push_ale,
	inst_undefined,
	inst_undefined,
	inst_pop_all,
	inst_pop_ale,
	inst_undefined,
	inst_undefined,
	inst_ld_ba_abshl,
	inst_ld_hl_abshl,
	inst_ld_ix_abshl,
	inst_ld_iy_abshl,
	inst_ld_abshl_ba,
	inst_ld_abshl_hl,
	inst_ld_abshl_ix,
	inst_ld_abshl_iy,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_ld_ba_absix,
	inst_ld_hl_absix,
	inst_ld_ix_absix,
	inst_ld_iy_absix,
	inst_ld_absix_ba,
	inst_ld_absix_hl,
	inst_ld_absix_ix,
	inst_ld_absix_iy,
	inst_ld_ba_absiy,
	inst_ld_hl_absiy,
	inst_ld_ix_absiy,
	inst_ld_iy_absiy,
	inst_ld_absiy_ba,
	inst_ld_absiy_hl,
	inst_ld_absiy_ix,
	inst_ld_absiy_iy,
	inst_ld_ba_ba,
	inst_ld_ba_hl,
	inst_ld_ba_ix,
	inst_ld_ba_iy,
	inst_ld_hl_ba,
	inst_ld_hl_hl,
	inst_ld_hl_ix,
	inst_ld_hl_iy,
	inst_ld_ix_ba,
	inst_ld_ix_hl,
	inst_ld_ix_ix,
	inst_ld_ix_iy,
	inst_ld_iy_ba,
	inst_ld_iy_hl,
	inst_ld_iy_ix,
	inst_ld_iy_iy,
	inst_ld_sp_ba,
	inst_ld_sp_hl,
	inst_ld_sp_ix,
	inst_ld_sp_iy,
	inst_ld_hl_sp,
	inst_ld_hl_pc,
	inst_undefined,
	inst_undefined,
	inst_ld_ba_sp,
	inst_ld_ba_pc,
	inst_ld_ix_sp,
	inst_undefined,
	inst_undefined,
	inst_undefined,
	inst_ld_iy_sp,
	inst_undefined,
};
