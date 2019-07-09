import Registers from "./registers";

export default class Minimon {
	async init() {
		const data = await fetch("./libminimon.wasm");
		this._module = await WebAssembly.instantiate(await data.arrayBuffer(), {
			env: {
				cpu_read_cart: (cpu, address) => this.cpu_read_cart(address),
				cpu_write_cart: (cpu, data, address) => this.cpu_write_cart(data, address)
			}
		});

		this._exports = this._module.instance.exports;
		this._cpu_state = this._exports.get_machine();
		this._machineView = new DataView(this._exports.memory.buffer, this._exports.get_machine());

		this.registers = new Registers(this._machineView);

		this.cartridge = new Uint8Array(0x200000);

		// Reset our CPU
		this.reset();

		let last = Date.now();
		
		const g = () => {
			let now = Date.now();
			console.log(now - last);
			last = now;
			setTimeout(g, 0)
		};

		g();
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
	}

	reset() {
		this._exports.cpu_reset(this._cpu_state);
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
