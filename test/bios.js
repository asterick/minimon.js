const fs = require("fs");
const path = require("path");

const { Disassembler, Minimon } = require("../public/system.compiled.js");

const system = new Minimon();
const disasm = new Disassembler(system);

const data = new Uint8Array(fs.readFileSync(path.join(__dirname, "../public/libminimon.wasm"))).buffer;

function hex16(v) {
	v = v.toString(16);
	return "0000".substr(v.length) + v;
}

function hex8(v) {
	v = v.toString(16);
	return "00".substr(v.length) + v;
}

function pad(s, l) {
	while (s.length < l) s += " ";
	return s;
}

const verified = [
 	0x38, 	// XOR A, A
 	0x44,	// LD A, [BR:nn]
 	0x48,	// LD B, A
 	0x78,	// LD [BR:nn], A
 	0x79,	// LD [BR:nn], B
 	0x82,	// INC L
 	0x92,	// INC IX
 	0x93,	// INC IY
	0x9A,	// DEC IX
	0x9F,	// LD SC, #nn
	0xA0,	// PUSH BA
	0xA2,	// PUSH IX
	0xB4,	// LD BR, #nn
	0xC6,	// LD IX, #nn
	0xD9,	// OR [BR:nn], #nn
	0xDD,	// LD [BR:nn], #nn
	0xE7,	// JNZ NZ, #rr
	0xF2,	// CARL #nnnn
	0xF8,	// RET

	0xCEBD,	// CP L, #nn
	0xCECD,	// LD EP, A
	0xCECE, // LD XP, A
	0xCECF, // LD YP, A

	0xCE4B, // LD B, [IY+L]
	0xCE4E, // LD [IX+L], B

	0xCF6E,	// LD SP, #nnnn
	0xCFE4, // LD HL, BA
	0xCFE8, // LD IX, BA
	0xCFEC, // LD IY, BA
	0xCEC5,	// LD EP, #nn
];

system.code = function () {
	const first = this.read(this.translate(this.registers.pc));

	if (first == 0xCE || first == 0xCF) {
		return (first << 8) | this.read(this.translate((this.registers.pc + 1) & 0xFFFF));
	}

	return first;
}

system.step = function() {
	disasm._address = this.registers.pc;

	const { ba, hl, ix, iy, pc, sp, br, ep, xp, yp, cb, nb, sc } = this.registers;
	if (verified.indexOf(this.code()) < 0) {
		const { data, op, args, address } = disasm.next();

		console.log(`${hex16(address)}: ${pad(data.map(hex8).join(" "), 11)}: ${pad(`${op} ${args.join(", ")}`, 32)} ; PC: ${hex16(pc)} BA: ${hex16(ba)} HL: ${hex16(hl)} IX: ${hex16(ix)} IY: ${hex16(iy)} SP: ${hex16(sp)} BR: ${hex8(br)} EP: ${hex8(ep)} XP: ${hex8(xp)} YP: ${hex8(yp)} CB: ${hex8(cb)} NB: ${hex8(nb)} SC: ${hex8(sc)}`);
	} else {
		//console.log(`${hex16(this.registers.pc)}:                                               ; PC: ${hex16(pc)} BA: ${hex16(ba)} HL: ${hex16(hl)} IX: ${hex16(ix)} IY: ${hex16(iy)} SP: ${hex16(sp)} BR: ${hex8(br)} EP: ${hex8(ep)} XP: ${hex8(xp)} YP: ${hex8(yp)} CB: ${hex8(cb)} NB: ${hex8(nb)} SC: ${hex8(sc)}`);
	}

	this._exports.cpu_step(this._cpu_state);
	this.update();
}

async function run_test() {
	await system.init(data);

	system.registers.ba = 0xFACE;
	system.registers.hl = 0xFACE;
	system.registers.ix = 0xFACE;
	system.registers.iy = 0xFACE;
	system.registers.sp = 0xFACE;
	system.registers.br = 0x55;
	system.registers.ep = 0x55;
	system.registers.xp = 0x55;
	system.registers.yp = 0x55;
	system.registers.cb = 0x55;
	system.registers.nb = 0x55;
	system.registers.sc = 0x55;

	// Before bios goes crazy
	while (system.registers.pc < 0x8000) {
		system.step();
	}
}

run_test();
