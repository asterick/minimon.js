import Bios from "./bios.js";

class Minimon {
	constructor() {

	}

	read(address, pc) {
		if (address < Bios.length) return Bios[address] | 0;

		return 0xCD;
	}
}

export default Minimon;
