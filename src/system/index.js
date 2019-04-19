class Minimon {
	async init() {
		const data = await fetch("./libminimon.wasm");
		this._module = await WebAssembly.instantiate(await data.arrayBuffer());

		this._exports = this._module.instance.exports;
		this._cpu_state = this._exports.get_machine();

		const bios = await fetch("bios.min");
		const bytes = await bios.arrayBuffer();

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
