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
import * as screen from "./screen";

const KEYBOARD_CODES:{[key: number]: number} = {
	67: 0b00000001,
	88: 0b00000010,
	90: 0b00000100,
	38: 0b00001000,
	40: 0b00010000,
	37: 0b00100000,
	39: 0b01000000,
	 8: 0b10000000
};

export const TRACING_FLAGS:{[key: string]: number} = {
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

export default class System {
	breakpoints: Array<number>;
	state: any;

	_cpu_state: number;
	_tracing: boolean;
	_running: boolean;
	_name: string;
	_inputState: number;
	_audio: Audio;
	_timer: any;

	_machineBytes: Uint8Array;
	_wasm_environment: any;
	_exports: any;

	constructor(name:string) {
		this._name = name;
		this._inputState = 0b1111111111; // No cartridge inserted, no IRQ
		this._audio = new Audio();
		this.breakpoints = [];
		screen.init(this);

		this._wasm_environment = {
			env: {
				audio_push: (address:number, frames:number) => {
					let samples = new Float32Array(this._exports.memory.buffer, address, frames);
					this._audio.push(samples);
				},
				flip_screen: (address: number) => {
					screen.repaint(this._machineBytes, address);
				},
				debug_print: (a: number) => {
					const str = [];
					let ch;
					while (ch = this._machineBytes[a++]) str.push(String.fromCharCode(ch));

					console.log(str.join(""));
				},
				trace_access: this.trace.bind(this)
			}
		}

		window.addEventListener("beforeunload", (e) => {
			this.preserve();
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

	preserve() {
		if (!this.state) return ;

		let { prescale, running, value } = this.state.rtc;
		let encoded = "";
		for (let i = 0; i < this.state.gpio.eeprom.data.length; i++) {
			encoded += String.fromCharCode(this.state.gpio.eeprom.data[i]);
		}

		window.localStorage.setItem(`${this._name}-eeprom`, encoded);
		window.localStorage.setItem(`${this._name}-rtc`, JSON.stringify({ prescale, running, value, timestamp: +Date.now()}));
	}

	restore() {
		if (!this.state) return ;

		try {
			let encoded = window.localStorage.getItem(`${this._name}-eeprom`);

			for (let i = 0; i < encoded.length; i++) {
				this.state.gpio.eeprom.data[i] = encoded.charCodeAt(i);
			}

			// Restore clock (if set)
			let rtc = JSON.parse(window.localStorage.getItem(`${this._name}-rtc`));
			let sec = Math.floor((+Date.now() - rtc.timestamp) / 1000)

			this.state.rtc.running = rtc.running;
			this.state.rtc.prescale = rtc.prescale;
			this.state.rtc.value = rtc.value + sec;

		} catch (e) {
			console.log("Could not restore system state");
		}
	}

	async init(tracing:boolean) {
		this._tracing = tracing;

		this.preserve();

		const request = await fetch(tracing ? "../static/libminimon.tracing.wasm" : "../static/libminimon.wasm");
		const wasm = await WebAssembly.instantiate(await request.arrayBuffer(), this._wasm_environment);
		this._exports = wasm.instance.exports;

		this._cpu_state = this._exports.get_machine();
		this._machineBytes = new Uint8Array(this._exports.memory.buffer);
		this.state = State(this._exports.memory.buffer, this._exports.get_description(), this._cpu_state);

		this._exports.set_sample_rate(this._cpu_state, this._audio.sampleRate);
		this.reset();
		this.restore();
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
			let delta = Math.floor(Math.min(200, now - time) * CPU_FREQ / 1000);

			time = now;

			if (this.breakpoints.length) {
				this.state.clocks += delta;	// advance our clock

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

		this.update();
	}

	// Trigger an update to the UI
	update() {
		// This will be overidden elsewhere
	}

	trace(cpu: number, address: number, kind: number, data: number) {
	}

	_updateinput() {
		this._exports.update_inputs(this._cpu_state, this._inputState);
	}

	// Cartridge I/O
	load (ab: ArrayBuffer) {
		var bytes = new Uint8Array(ab);
		var hasHeader = (bytes[0] != 0x50 || bytes[1] != 0x4D);
		var offset = hasHeader ? 0 : 0x2100;


		setTimeout(() => {
			for (let i = bytes.length - 1; i >= 0; i--) this.state.cartridge[(i+offset) & 0x1FFFFF] = bytes[i];
			this._inputState &= ~INPUT_CART_N;
			this._updateinput();
		}, 100);

		this.eject();
	}

	eject() {
		this._inputState |= INPUT_CART_N;
		this._updateinput();
	}

	// WASM shim functions
	translate(address: number) {
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
		this._updateinput();
		this.update();
	}

	read(address: number) {
		return this._exports.cpu_read(this._cpu_state, address);
	}

	write(data: number, address: number) {
		return this._exports.cpu_write(this._cpu_state, data, address);
	}
}
