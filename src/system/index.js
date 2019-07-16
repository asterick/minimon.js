import Registers from "./registers";

export default class Minimon {
	async init() {
		const data = await fetch("./libminimon.wasm");
		this._module = await WebAssembly.instantiate(await data.arrayBuffer(), {
			env: {
				cpu_read_cart: (cpu, address) => this.cpu_read_cart(address),
				cpu_write_cart: (cpu, data, address) => this.cpu_write_cart(data, address),
				debug_print: (... rest) => this.debug_print.apply(this, rest)
			}
		});

		this._exports = this._module.instance.exports;
		this._cpu_state = this._exports.get_machine();
		this._machineBytes = new Uint8Array(this._exports.memory.buffer);

		this.registers = new Registers(this._exports.memory.buffer, this._cpu_state);

		this.cartridge = new Uint8Array(0x200000);

		// Reset our CPU
		this.reset();
	}

	debug_print(a) {
		const str = [];
		let ch;
		while (ch = this._machineBytes[a++]) str.push(String.fromCharCode(ch));

		console.log(str.join(""));
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
