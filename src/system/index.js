class Minimon {
	async init() {
		const data = await fetch("./libminimon.wasm");
		this._module = await WebAssembly.instantiate(await data.arrayBuffer());

		this._exports = this._module.instance.exports;
		this._cpu_state = this._exports.get_machine();
		this._machineView = new DataView(this._exports.memory.buffer, this._exports.get_machine());

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

	translate(address) {
		return this._exports.cpu_translate(this._cpu_state, address);
	}

	get ba() {
		return this._machineView.getUint16(0, true);
	}

	get hl() {
		return this._machineView.getUint16(2, true);
	}

	get ix() {
		return this._machineView.getUint16(4, true);
	}

	get iy() {
		return this._machineView.getUint16(6, true);
	}

	get pc() {
		return this._machineView.getUint16(8, true);
	}

	get sp() {
		return this._machineView.getUint16(10, true);
	}

	get br() {
		return this._machineView.getUint8(12);
	}

	get ep() {
		return this._machineView.getUint8(13);
	}

	get xp() {
		return this._machineView.getUint8(14);
	}

	get yp() {
		return this._machineView.getUint8(15);
	}

	get cb() {
		return this._machineView.getUint8(16);
	}

	get nb() {
		return this._machineView.getUint8(17);
	}

	get sc() {
		return this._machineView.getUint8(18);
	}
}

export default Minimon;
