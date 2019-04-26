export default class Registers {
	constructor(dv) {
		this._machineView = dv;
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