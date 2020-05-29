/*
ISC License

Copyright (c) 2019, Bryon Vandiver

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

import State from "./state";
import Audio from "./audio";

const KEYBOARD_CODES = {
	67: 0b00000001,
	88: 0b00000010,
	90: 0b00000100,
	38: 0b00001000,
	40: 0b00010000,
	37: 0b00100000,
	39: 0b01000000,
	 8: 0b10000000
};

export const TRACING_FLAGS = {
    INSTRUCTION:   0b00000001,
    IMMEDIATE:     0b00000010,
    BRANCH_TARGET: 0b00000100,
    TILE_DATA:     0b00001000,
    SPRITE_DATA:   0b00010000,
    READ:          0b01000000,
    WRITE:         0b10000000
};

const INPUT_CART_N = 0b1000000000;
const CPU_FREQ = 4000000;

export class Minimon {
	constructor(name) {
		this._name = name;
		this._inputState = 0b1111111111; // No cartridge inserted, no IRQ
		this._audio = new Audio();
		this.cartridge = new Uint8Array(0x200000);
		this.breakpoints = [];

		window.addEventListener("beforeunload", (e) => {
			this.preserveEEPROM();
		}, false);

		document.body.addEventListener('keydown', (e) => {
			this._inputState &= ~KEYBOARD_CODES[e.keyCode];
			this._updateinput();
		});

		document.body.addEventListener('keyup', (e) => {
			this._inputState |= KEYBOARD_CODES[e.keyCode];
			this._updateinput();
		});
	}

	preserveEEPROM() {
		if (!this.state) return ;

		let encoded = "";
		for (let i = 0; i < 0x2000; i++) {
			encoded += String.fromCharCode(this.state.gpio.eeprom.data[i]);
		}

		window.localStorage.setItem(`${this._name}-eeprom`, encoded);
	}

	restoreEEPROM() {
		if (!this.state) return ;

		let encoded = window.localStorage.getItem(`${this._name}-eeprom`);

		for (let i = 0; i < 0x2000; i++) {
			this.state.gpio.eeprom.data[i] = encoded.charCodeAt(i);
		}
	}

	async init(tracing) {
		this._tracing = tracing;

		// TODO: RESTORE EEPROM HERE
		if (this.state) {
			this.preserveEEPROM();
		}

		const request = await fetch(tracing ? "./libminimon.tracing.wasm" : "./libminimon.wasm");
		const wasm = await WebAssembly.instantiate(await request.arrayBuffer(), this._wasm_environment);
		this._exports = wasm.instance.exports;

		this._cpu_state = this._exports.get_machine();
		this._machineBytes = new Uint8Array(this._exports.memory.buffer);
		this.state = new State(this._exports.memory.buffer, this._exports.get_description());

		this.restoreEEPROM();
		this.reset();
	}

	_wasm_environment = {
		env: {
			cpu_read_cart: (cpu, address) => this.cpu_read_cart(address),
			cpu_write_cart: (cpu, data, address) => this.cpu_write_cart(data, address),
			flip_screen: (address) => {
				this.repaint(this._machineBytes, address);
			},
			debug_print: (a) => {
				const str = [];
				let ch;
				while (ch = this._machineBytes[a++]) str.push(String.fromCharCode(ch));

				console.log(str.join(""));
			},
			trace_access: (cpu, address, kind) => this.trace(cpu, address, kind)
		}
	}

	get tracing() {
		return this._tracing;
	}

	set tracing(v) {
		if (this._tracing == v) return ;

		this.init(v);
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
			let delta = Math.min(200, now - time);

			time = now;

			if (this.breakpoints.length) {
				this.state.clocks += (delta * CPU_FREQ / 1000) | 0;	// advance our clock

				while (this.state.clocks > 0) {
					if (this.breakpoints.indexOf(this.translate(this.state.cpu.pc)) >= 0) {
						this.running = false;
						break ;
					}

					this._exports.cpu_step(this._cpu_state);
				}
			} else {
				this._exports.cpu_advance(this._cpu_state, delta);
			}
			this.update();
		}

		this._running = v;

		if (v) {
			this._timer = setInterval(_tick, 0);
		} else {
			clearInterval(this._timer);
		}
	}

	// Trigger an update to the UI
	repaint() {

	}

	update() {
		// This will be overidden elsewhere
	}

	trace(cpu, address, kind) {

	}

	_updateinput(v) {
		this._exports.update_inputs(this._cpu_state, this._inputState);
	}

	// Cartridge I/O
	load (ab) {
		var bytes = new Uint8Array(ab);
		var hasHeader = (bytes[0] != 0x50 || bytes[1] != 0x4D);
		var offset = hasHeader ? 0 : 0x2100;

		for (let i = bytes.length - 1; i >= 0; i--) this.cartridge[(i+offset) & 0x1FFFFF] = bytes[i];

		setTimeout(() => {
			this._inputState &= ~INPUT_CART_N;
			this._updateinput();
		}, 100);

		this.eject();
	}

	eject() {
		this._inputState |= INPUT_CART_N;
		this._updateinput();
	}

	cpu_read_cart(address) {
		return this.cartridge[address & 0x1FFFFF];
	}

	cpu_write_cart(data, address) {
		// ROM, no multi-cart support yet
	}

	// WASM shim functions
	translate(address) {
		if (address & 0x8000) {
			return (address & 0x7FFF) | (this.state.cpu.cb << 15);
		} else {
			return address;
		}
	}

	step() {
		this._exports.cpu_step(this._cpu_state);
		this.update();
	}

	reset() {
		this._exports.cpu_reset(this._cpu_state);
		this.update();
	}

	read(address) {
		return this._exports.cpu_read(this._cpu_state, address);
	}

	write(data, address) {
		return this._exports.cpu_write(this._cpu_state, data, address);
	}
}
