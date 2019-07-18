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

system.step = function() {
	disasm._address = this.registers.pc;

	const { data, op, args, address } = disasm.next();
	const { ba, hl, ix, iy, pc, sp, br, ep, xp, yp, cb, nb, sc } = this.registers;

	console.log(`${hex16(address)}: ${pad(data.map(hex8).join(" "), 11)}: ${pad(`${op} ${args.join(", ")}`, 32)} ; BA: ${hex16(ba)} HL: ${hex16(hl)} IX: ${hex16(ba)} IY: ${hex16(ba)} PC: ${hex16(pc)} SP: ${hex16(sp)} BR: ${hex8(br)} XP: ${hex8(xp)} YP: ${hex8(yp)} BR: ${hex8(br)} CB: ${hex8(cb)} NB: ${hex8(nb)} SC: ${hex8(sc)}`);

	this._exports.cpu_step(this._cpu_state);
	this.update();
}

async function run_test() {
	await system.init(data);

	// Before bios goes crazy
	while (system.registers.pc < 0x8000) {
		system.step();
	}
}

run_test();
