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

const KEYBOARD_CODES:{[key: string]: number} = {
	"c":			0b00000001,
	"x":			0b00000010,
	"z":			0b00000100,
	"ArrowUp":		0b00001000,
	"ArrowDown":	0b00010000,
	"ArrowLeft":	0b00100000,
	"ArrowRight":	0b01000000,
	"Backspace":	0b10000000
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

let breakpoints: Array<number> = [];
let state: any;

let inputState = 0b1111111111;

let cpu_state: number;
let running: boolean;
let time: number;
let audio: Audio;
let timer: ReturnType<typeof setInterval>;

let machineBytes: Uint8Array;
let exported: any;

const name = "default";
const wasm_environment = {
	env: {
		audio_push: (address:number, frames:number) => {
			const samples = new Float32Array(exported.memory.buffer, address, frames);
			audio.push(samples);
		},
		flip_screen: (address: number) => {
			screen.repaint(machineBytes, address);
		},
		debug_print: (a: number) => {
			const str = [];
			for(;;) {
				const ch = machineBytes[a++];
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

export async function init(): Promise<void> {
	audio = new Audio();

	window.addEventListener("beforeunload", preserve, false);

	preserve();

	const request = await fetch("../static/libminimon.wasm");
	const wasm = await WebAssembly.instantiate(await request.arrayBuffer(), wasm_environment);
	exported = wasm.instance.exports;

	cpu_state = exported.get_machine();
	machineBytes = new Uint8Array(exported.memory.buffer);
	state = State(exported.memory.buffer, exported.get_description(), cpu_state);

	exported.set_sample_rate(cpu_state, audio.sampleRate);
	reset();
	restore();

	document.body.addEventListener('keydown', (e: KeyboardEvent) => {
		inputState &= ~KEYBOARD_CODES[e.key];
		updateinput();
	});

	document.body.addEventListener('keyup', (e: KeyboardEvent) => {
		inputState |= KEYBOARD_CODES[e.key];
		updateinput();
	});
}

function preserve() {
	if (!state) return ;

	const { prescale, running, value } = state.rtc;
	let encoded = "";
	for (let i = 0; i < state.gpio.eeprom.data.length; i++) {
		encoded += String.fromCharCode(state.gpio.eeprom.data[i]);
	}

	window.localStorage.setItem(`${name}-eeprom`, encoded);
	window.localStorage.setItem(`${name}-rtc`, JSON.stringify({ prescale, running, value, timestamp: +Date.now()}));
}

function restore() {
	if (!state) return ;

	try {
		const encoded = window.localStorage.getItem(`${name}-eeprom`);

		for (let i = 0; i < encoded.length; i++) {
			state.gpio.eeprom.data[i] = encoded.charCodeAt(i);
		}

		// Restore clock (if set)
		const rtc = JSON.parse(window.localStorage.getItem(`${name}-rtc`));
		const sec = Math.floor((+Date.now() - rtc.timestamp) / 1000)

		state.rtc.running = rtc.running;
		state.rtc.prescale = rtc.prescale;
		state.rtc.value = rtc.value + sec;

	} catch (e) {
		console.log("Could not restore system state");
	}
}

function tick() {
	if (!running) return ;

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

			exported.cpu_step(cpu_state);
		}
	} else {
		exported.cpu_advance(cpu_state, delta);
	}

	update();
}

// Trigger an update to the UI
function update() {
	// This will be overidden elsewhere
}

export function start(): void {
	if (running) return ;
	running = true;

	time = Date.now();
	timer = setInterval(tick, 1);

	update();
}

export function stop(): void {
	if (!running) return ;
	running = false;

	clearInterval(timer);
}

export function updateinput(): void {
	exported.update_inputs(cpu_state, inputState);
}

// Cartridge I/O
export function load (ab: ArrayBuffer): void {
	const bytes = new Uint8Array(ab);
	const hasHeader = (bytes[0] != 0x50 || bytes[1] != 0x4D);
	const offset = hasHeader ? 0 : 0x2100;

	setTimeout(() => {
		for (let i = bytes.length - 1; i >= 0; i--) state.cartridge[(i+offset) & 0x1FFFFF] = bytes[i];
		inputState &= ~INPUT_CART_N;
		updateinput();
	}, 100);

	eject();
}

export function eject(): void {
	inputState |= INPUT_CART_N;
	updateinput();
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
	exported.cpu_step(cpu_state);
	update();
}

export function reset(): void {
	exported.cpu_reset(cpu_state);
	updateinput();
	update();
}

export function read(address: number): number {
	return exported.cpu_read(cpu_state, address);
}

export function write(data: number, address: number): void {
	exported.cpu_write(cpu_state, data, address);
}
