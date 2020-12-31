/*
 * MIT License
 *
 * Copyright (c) 20192-2021, Bryon Vandiver
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

import State from "./state";
import Audio from "./audio";
import * as screen from "./screen";

const KEYBOARD_CODES:{[key: number]: number} = {
	67:	0b00000001,
	88:	0b00000010,
	90:	0b00000100,
	38:	0b00001000,
	40:	0b00010000,
	37:	0b00100000,
	39:	0b01000000,
	8:	0b10000000
};

export const TRACING_FLAGS:{[key: string]: number} = {
	INSTRUCTION:	0b00000001,
	IMMEDIATE:		0b00000010,
	BRANCH_TARGET:	0b00000100,
	TILE_DATA:		0b00001000,
	SPRITE_DATA:	0b00010000,
	READ:			0b01000000,
	WRITE:			0b10000000
};

const INPUT_CART_N = 0b1000000000;
const CPU_FREQ = 4000000;

let breakpoints: Array<number>;
let state: any;

let _cpu_state: number;
let _running: boolean;
let time: number;
let _inputState: number;
let _audio: Audio;
let _timer: ReturnType<typeof setInterval>;

let _machineBytes: Uint8Array;
let _exports: any;

const _name = "default";
const _wasm_environment = {
	env: {
		audio_push: (address:number, frames:number) => {
			const samples = new Float32Array(_exports.memory.buffer, address, frames);
			_audio.push(samples);
		},
		flip_screen: (address: number) => {
			screen.repaint(_machineBytes, address);
		},
		debug_print: (a: number) => {
			const str = [];
			for(;;) {
				const ch = _machineBytes[a++];
				if (!ch) break ;
				str.push(String.fromCharCode(ch));
			}

			console.log(str.join(""));
		},
		trace_access: (cpu: number, address: number, kind: number, data: number) => {
			// TODO
		}
	}
};

export async function init(tracing:boolean): Promise<void> {
	_inputState = 0b1111111111; // No cartridge inserted, no IRQ
	_audio = new Audio();
	breakpoints = [];
	screen.init();

	window.addEventListener("beforeunload", preserve, false);

	preserve();

	const request = await fetch(tracing ? "../static/libminimon.tracing.wasm" : "../static/libminimon.wasm");
	const wasm = await WebAssembly.instantiate(await request.arrayBuffer(), _wasm_environment);
	_exports = wasm.instance.exports;

	_cpu_state = _exports.get_machine();
	_machineBytes = new Uint8Array(_exports.memory.buffer);
	state = State(_exports.memory.buffer, _exports.get_description(), _cpu_state);

	_exports.set_sample_rate(_cpu_state, _audio.sampleRate);
	reset();
	restore();

	document.body.addEventListener('keydown', (e: KeyboardEvent) => {
		_inputState &= ~KEYBOARD_CODES[e.keyCode];
		_updateinput();
	});

	document.body.addEventListener('keyup', (e: KeyboardEvent) => {
		_inputState |= KEYBOARD_CODES[e.keyCode];
		_updateinput();
	});
}

function preserve() {
	if (!state) return ;

	const { prescale, running, value } = state.rtc;
	let encoded = "";
	for (let i = 0; i < state.gpio.eeprom.data.length; i++) {
		encoded += String.fromCharCode(state.gpio.eeprom.data[i]);
	}

	window.localStorage.setItem(`${_name}-eeprom`, encoded);
	window.localStorage.setItem(`${_name}-rtc`, JSON.stringify({ prescale, running, value, timestamp: +Date.now()}));
}

function restore() {
	if (!state) return ;

	try {
		const encoded = window.localStorage.getItem(`${_name}-eeprom`);

		for (let i = 0; i < encoded.length; i++) {
			state.gpio.eeprom.data[i] = encoded.charCodeAt(i);
		}

		// Restore clock (if set)
		const rtc = JSON.parse(window.localStorage.getItem(`${_name}-rtc`));
		const sec = Math.floor((+Date.now() - rtc.timestamp) / 1000)

		state.rtc.running = rtc.running;
		state.rtc.prescale = rtc.prescale;
		state.rtc.value = rtc.value + sec;

	} catch (e) {
		console.log("Could not restore system state");
	}
}

function tick() {
	if (!_running) return ;

	const now = Date.now();
	const delta = Math.floor(Math.min(200, now - time) * CPU_FREQ / 1000);

	time = now;

	if (breakpoints.length) {
		state.clocks += delta;	// advance our clock

		while (state.clocks > 0) {
			if (breakpoints.indexOf(translate(state.cpu.pc)) >= 0) {
				stop();
				break ;
			}

			_exports.cpu_step(_cpu_state);
		}
	} else {
		_exports.cpu_advance(_cpu_state, delta);
	}

	update();
}

// Trigger an update to the UI
function update() {
	// This will be overidden elsewhere
}

export function start(): void {
	if (_running) return ;
	_running = true;

	time = Date.now();
	_timer = setInterval(tick, 1);

	update();
}

export function stop(): void {
	if (!_running) return ;
	_running = false;

	clearInterval(_timer);
}

export function _updateinput(): void {
	_exports.update_inputs(_cpu_state, _inputState);
}

// Cartridge I/O
export function load (ab: ArrayBuffer): void {
	const bytes = new Uint8Array(ab);
	const hasHeader = (bytes[0] != 0x50 || bytes[1] != 0x4D);
	const offset = hasHeader ? 0 : 0x2100;

	setTimeout(() => {
		for (let i = bytes.length - 1; i >= 0; i--) state.cartridge[(i+offset) & 0x1FFFFF] = bytes[i];
		_inputState &= ~INPUT_CART_N;
		_updateinput();
	}, 100);

	eject();
}

export function eject(): void {
	_inputState |= INPUT_CART_N;
	_updateinput();
}

// WASM shim functions
export function translate(address: number): number {
	if (address & 0x8000) {
		return (address & 0x7FFF) | (state.cpu.cb << 15);
	} else {
		return address;
	}
}

export function step(): void {
	_exports.cpu_step(_cpu_state);
	update();
}

export function reset(): void {
	_exports.cpu_reset(_cpu_state);
	_updateinput();
	update();
}

export function read(address: number): number {
	return _exports.cpu_read(_cpu_state, address);
}

export function write(data: number, address: number): void {
	_exports.cpu_write(_cpu_state, data, address);
}
