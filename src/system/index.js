import Registers from "./registers";

class Minimon {
	async init() {
		const data = await fetch("./libminimon.wasm");
		this._module = await WebAssembly.instantiate(await data.arrayBuffer());

		this._exports = this._module.instance.exports;
		this._cpu_state = this._exports.get_machine();
		this._machineView = new DataView(this._exports.memory.buffer, this._exports.get_machine());

		this.registers = new Registers(this._machineView);

		// Reset our CPU
		this.reset();
	}	

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

export default Minimon;
