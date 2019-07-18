import Registers from "./registers";

import disasm from "./disassemble";

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

export default class Minimon {
	async init() {
		const data = await fetch("./libminimon.wasm");
		this._module = await WebAssembly.instantiate(await data.arrayBuffer(), {
			env: {
				cpu_read_cart: (cpu, address) => this.cpu_read_cart(address),
				cpu_write_cart: (cpu, data, address) => this.cpu_write_cart(data, address),
				debug_print: (a) => {
					const str = [];
					let ch;
					while (ch = this._machineBytes[a++]) str.push(String.fromCharCode(ch));

					console.log(str.join(""));
				}
			}
		});

		this._exports = this._module.instance.exports;
		this._cpu_state = this._exports.get_machine();
		this._machineBytes = new Uint8Array(this._exports.memory.buffer);

		this.registers = new Registers(this._exports.memory.buffer, this._cpu_state);

		this.cartridge = new Uint8Array(0x200000);

		// Reset our CPU
		this.reset();

		// This is when it all falls apart
		const contains = [];
		this._disasm = new disasm(this);

		while (this.registers.pc < 0x8000) {
			const addy = this.registers.pc;
			if (contains.indexOf(addy) < 0) {
				this._disasm._address = this.registers.pc;
				const { data, op, args, address } = this._disasm.next();

				//console.log("----------------------------------------------------------------")
				//this.dump_regs();
				console.log(`${address.toString(16)}: ${pad(data.map(hex8).join(" "), 11)}: ${op} ${args.join(", ")}`);
				//this.dump_regs();

				contains.push(addy)
			}
			
			this.step();
		}
	}

	dump_regs() {
		const { ba, hl, ix, iy, pc, sp, br, ep, xp, yp, cb, nb, sc } = this.registers;
		console.log(`BA: ${hex16(ba)} HL: ${hex16(hl)} IX: ${hex16(ba)} IY: ${hex16(ba)} PC: ${hex16(pc)} SP: ${hex16(sp)} `)
		console.log(`BR: ${hex8(br)} XP: ${hex8(xp)} YP: ${hex8(yp)} BR: ${hex8(br)} CB: ${hex8(cb)} NB: ${hex8(nb)} SC: ${hex8(sc)} `);
	}

	get running() {
		return this._running;
	}

	set running(v) {
		if (this._running == v) return ;

		let time = Date.now();

		const _tick = () => {
			if (!this._running) return ;

			let now = Date.now();
			let delta = Math.min(50, now - time);


			this._exports.cpu_advance(this._cpu_state, delta);
			time = now;

			this.update();
		}

		this._running = v;

		if (v) {
			this._timer = setInterval(_tick, 0);
		} else {
			clearInterval(this._timer);
		}

		this.update();
	}

	// Trigger an update to the UI
	update() {
		// This will be overidden elsewhere
	}

	// Cartridge I/O
	load (ab) {
		var bytes = new Uint8Array(ab);
		var hasHeader = (bytes[0] != 0x4D || bytes[1] != 0x4E);
		var offset = hasHeader ? 0 : 0x2100;

		for (let i = bytes.length - 1; i >= 0; i--) this.cartridge[(i+offset) & 0x1FFFFF] = bytes[i];

		this.reset();		
	}

	cpu_read_cart(address) {
		return this.cartridge[address & 0x1FFFFF];
	}

	cpu_write_cart(data, address) {
		// Unsupported
	}

	trace() {
		const old_step = this.step;

		this._disasm = new disasm(this);
		this.step = () => {
			this._disasm._address = this.registers.pc;
			const { op, args, address } = this._disasm.next();
			console.log(`${address.toString(16)}: ${op} ${args.join(", ")}`);

			old_step.call(this);
		}
	}

	// WASM shim functions
	step() {
		this._exports.cpu_step(this._cpu_state);
		this.update();
	}

	reset() {
		this._exports.cpu_reset(this._cpu_state);
		this.update();

		this.running = false;
	}

	read(address) {
		return this._exports.cpu_read8(this._cpu_state, address);
	}

	write(data, address) {
		return this._exports.cpu_read8(this._cpu_state, data, address);
	}

	translate(address) {
		return this._exports.cpu_translate(this._cpu_state, address);
	}
}
