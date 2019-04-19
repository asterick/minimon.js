import Bios from "./bios.js";

class Minimon {
	async init() {
		const data = await fetch("./libminimon.wasm");

		this._module = await WebAssembly.instantiate(await data.arrayBuffer(), {
			"env": {
				cpu_read8: () => 0,
				cpu_write8: () => 0
			}
		});

		this._exports = this._module.instance.exports;
	}	

	read(address) {

	}

	translate(pc) {
		return pc;
	}
}

export default Minimon;
