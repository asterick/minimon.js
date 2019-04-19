class Minimon {
	async init() {
		const data = await fetch("./libminimon.wasm");
		this._module = await WebAssembly.instantiate(await data.arrayBuffer());

		this._exports = this._module.instance.exports;
		this._cpu_state = this._exports.get_machine();

		// Copy in our bios image
		const biosSource = new Uint8Array(await (await fetch("bios.min")).arrayBuffer());
		const biosTarget = new Uint8Array(this._exports.memory.buffer, this._exports.get_bios(), biosSource.length);

		for (let i = 0; i < biosSource.length; i++) {
			biosTarget[i] = biosSource[i];
		}

		// Reset our CPU
		this.reset();
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
}

export default Minimon;
