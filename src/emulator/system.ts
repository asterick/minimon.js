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

let breakpoints: Array<number>;
let state: any;

let _cpu_state: number;
let _tracing: boolean;
let _running: boolean;
let time: number;
let _inputState: number;
let _audio: Audio;
let _timer: any;

let _machineBytes: Uint8Array;
let _exports: any;

const _name: string = "default";
const _wasm_environment: any = {
	env: {
		audio_push: (address:number, frames:number) => {
			let samples = new Float32Array(_exports.memory.buffer, address, frames);
			_audio.push(samples);
		},
		flip_screen: (address: number) => {
			screen.repaint(_machineBytes, address);
		},
		debug_print: (a: number) => {
			const str = [];
			let ch;
			while (ch = _machineBytes[a++]) str.push(String.fromCharCode(ch));

			console.log(str.join(""));
		},
		trace_access: trace.bind(this)
	}
};

export async function init(tracing:boolean) {
	_inputState = 0b1111111111; // No cartridge inserted, no IRQ
	_audio = new Audio();
	breakpoints = [];
	screen.init();

	window.addEventListener("beforeunload", (e) => {
		preserve();
	}, false);

	document.body.addEventListener('keydown', (e) => {
		_inputState &= ~KEYBOARD_CODES[e.keyCode];
		_updateinput();
	});

	document.body.addEventListener('keyup', (e) => {
		_inputState |= KEYBOARD_CODES[e.keyCode];
		_updateinput();
	});

	_tracing = tracing;

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
}

function preserve() {
	if (!state) return ;

	let { prescale, running, value } = state.rtc;
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
		let encoded = window.localStorage.getItem(`${_name}-eeprom`);

		for (let i = 0; i < encoded.length; i++) {
			state.gpio.eeprom.data[i] = encoded.charCodeAt(i);
		}

		// Restore clock (if set)
		let rtc = JSON.parse(window.localStorage.getItem(`${_name}-rtc`));
		let sec = Math.floor((+Date.now() - rtc.timestamp) / 1000)

		state.rtc.running = rtc.running;
		state.rtc.prescale = rtc.prescale;
		state.rtc.value = rtc.value + sec;

	} catch (e) {
		console.log("Could not restore system state");
	}
}

function tick() {
	if (!_running) return ;

	let now = Date.now();
	let delta = Math.floor(Math.min(200, now - time) * CPU_FREQ / 1000);

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

function trace(cpu: number, address: number, kind: number, data: number) {
}

export function start() {
	if (_running) return ;
	_running = true;

	let time = Date.now();
	_timer = setInterval(tick, 0);

	update();
}

export function stop() {
	if (!_running) return ;
	_running = false;

	clearInterval(_timer);
}

export function _updateinput() {
	_exports.update_inputs(_cpu_state, _inputState);
}

// Cartridge I/O
export function load (ab: ArrayBuffer) {
	var bytes = new Uint8Array(ab);
	var hasHeader = (bytes[0] != 0x50 || bytes[1] != 0x4D);
	var offset = hasHeader ? 0 : 0x2100;


	setTimeout(() => {
		for (let i = bytes.length - 1; i >= 0; i--) state.cartridge[(i+offset) & 0x1FFFFF] = bytes[i];
		_inputState &= ~INPUT_CART_N;
		_updateinput();
	}, 100);

	eject();
}

export function eject() {
	_inputState |= INPUT_CART_N;
	_updateinput();
}

// WASM shim functions
export function translate(address: number) {
	if (address & 0x8000) {
		return (address & 0x7FFF) | (state.cpu.cb << 15);
	} else {
		return address;
	}
}

export function step() {
	_exports.cpu_step(_cpu_state);
	update();
}

export function reset() {
	_exports.cpu_reset(_cpu_state);
	_updateinput();
	update();
}

export function read(address: number) {
	return _exports.cpu_read(_cpu_state, address);
}

export function write(data: number, address: number) {
	return _exports.cpu_write(_cpu_state, data, address);
}
