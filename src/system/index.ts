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

import struct from "./state";
import Audio from "./audio";
import { loadMap, saveMap, cartridgeKey, BIOS_KEY } from "./trace-store";

import type { MachineState } from "./machine-state";

export type { MachineState };

const KEYBOARD_CODES: Record<number, number> = {
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
} as const;

const INPUT_CART_N = 0b1000000000;
const CPU_FREQ = 4000000;

// C ABI exported by core/wasm/main.cc (see EXPORTS in core/wasm/Makefile)
interface CoreExports {
	memory: WebAssembly.Memory;
	get_machine(): number;
	get_description(): number;
	set_sample_rate(machine: number, sampleRate: number): void;
	update_inputs(machine: number, inputs: number): void;
	cpu_reset(machine: number): void;
	cpu_advance(machine: number, cycles: number): void;
	cpu_step(machine: number): void;
	cpu_read(machine: number, address: number): number;
	cpu_write(machine: number, data: number, address: number): void;
	get_bios_trace(): number;
	get_cartridge_trace(): number;
	get_bios(): number;
}

const BIOS_TRACE_SIZE = 0x1000;
const CARTRIDGE_TRACE_SIZE = 0x200000;
const BIOS_SIZE = 0x1000;
const TRACE_SAVE_INTERVAL = 10000;

export class Minimon {
	state!: MachineState;
	breakpoints = new Set<number>();

	// Classification maps maintained by the core (one TraceType flag
	// byte per address): the BIOS map is universal, the cartridge map
	// is cleared whenever the cartridge changes
	biosTrace!: Uint8Array;
	cartridgeTrace!: Uint8Array;

	// Read-only view of the embedded BIOS, for side-effect-free
	// debugger rendering
	bios!: Uint8Array;

	private _cartKey: string | null = null;

	private _listeners = new Set<() => void>();
	private _version = 0;
	private _notifyScheduled = false;

	private _name: string;
	private _inputState: number;
	private _audio: Audio;
	private _running = false;
	private _animation: number | undefined;
	private _exports!: CoreExports;
	private _cpu_state!: number;
	private _machineBytes!: Uint8Array;

	constructor(name: string) {
		this._name = name;
		this._inputState = 0b1111111111; // No cartridge inserted, no IRQ
		this._audio = new Audio();

		window.addEventListener("beforeunload", () => {
			this.preserve();
		}, false);

		// IndexedDB writes are async and can't ride beforeunload, so
		// classification persists on an interval and when the tab hides
		setInterval(() => this.preserveTrace(), TRACE_SAVE_INTERVAL);
		document.addEventListener("visibilitychange", () => {
			if (document.visibilityState === "hidden") this.preserveTrace();
		});

		document.body.addEventListener('keydown', (e) => {
			this._inputState &= ~(KEYBOARD_CODES[e.keyCode] ?? 0);
			this._updateinput();
		});

		document.body.addEventListener('keyup', (e) => {
			this._inputState |= KEYBOARD_CODES[e.keyCode] ?? 0;
			this._updateinput();
		});
	}

	preserve(): void {
		if (!this.state) return;

		const { prescale, running, value } = this.state.rtc;
		let encoded = "";
		for (let i = 0; i < this.state.gpio.eeprom.data.length; i++) {
			encoded += String.fromCharCode(this.state.gpio.eeprom.data[i]);
		}

		window.localStorage.setItem(`${this._name}-eeprom`, encoded);
		window.localStorage.setItem(`${this._name}-rtc`, JSON.stringify({ prescale, running, value, timestamp: +Date.now() }));
		window.localStorage.setItem(`${this._name}-breakpoints`, JSON.stringify([...this.breakpoints]));
	}

	restore(): void {
		if (!this.state) return;

		try {
			const breakpoints = JSON.parse(window.localStorage.getItem(`${this._name}-breakpoints`) ?? "[]");
			this.breakpoints = new Set(breakpoints.filter((v: unknown) => typeof v === "number"));
		} catch (e) {
			console.log("Could not restore breakpoints");
		}

		try {
			const encoded = window.localStorage.getItem(`${this._name}-eeprom`)!;

			for (let i = 0; i < encoded.length; i++) {
				this.state.gpio.eeprom.data[i] = encoded.charCodeAt(i);
			}

			// Restore clock (if set)
			const rtc = JSON.parse(window.localStorage.getItem(`${this._name}-rtc`)!);
			const sec = Math.floor((+Date.now() - rtc.timestamp) / 1000);

			this.state.rtc.running = rtc.running;
			this.state.rtc.prescale = rtc.prescale;
			this.state.rtc.value = rtc.value + sec;

		} catch (e) {
			console.log("Could not restore system state");
		}
	}

	async init(): Promise<void> {
		this.preserve();

		const request = await fetch("./libminimon.wasm");
		const wasm = await WebAssembly.instantiate(await request.arrayBuffer(), this._wasm_environment);
		this._exports = wasm.instance.exports as unknown as CoreExports;

		this._cpu_state = this._exports.get_machine();
		this._machineBytes = new Uint8Array(this._exports.memory.buffer);
		this.state = struct(this._exports.memory.buffer, this._exports.get_description(), this._cpu_state) as unknown as MachineState;

		this.biosTrace = new Uint8Array(this._exports.memory.buffer, this._exports.get_bios_trace(), BIOS_TRACE_SIZE);
		this.cartridgeTrace = new Uint8Array(this._exports.memory.buffer, this._exports.get_cartridge_trace(), CARTRIDGE_TRACE_SIZE);
		this.bios = new Uint8Array(this._exports.memory.buffer, this._exports.get_bios(), BIOS_SIZE);

		// The BIOS classification is universal; pick up where any
		// previous session left off
		try {
			const saved = await loadMap(BIOS_KEY);
			if (saved && saved.length === this.biosTrace.length) {
				this.biosTrace.set(saved);
			}
		} catch (e) {
			console.warn("Could not restore BIOS classification", e);
		}

		this._exports.set_sample_rate(this._cpu_state, this._audio.sampleRate);
		this.reset();
		this.restore();
	}

	// Classification maps persist across sessions: the BIOS map always,
	// the cartridge map keyed by ROM hash
	preserveTrace(): void {
		if (!this.biosTrace) return;

		void saveMap(BIOS_KEY, this.biosTrace).catch(() => {});
		if (this._cartKey) {
			void saveMap(this._cartKey, this.cartridgeTrace).catch(() => {});
		}
	}

	private _wasm_environment = {
		env: {
			audio_push: (address: number, frames: number) => {
				const samples = new Float32Array(this._exports.memory.buffer, address, frames);
				this._audio.push(samples);
			},
			flip_screen: (address: number) => {
				this.repaint(this._machineBytes, address);
			},
			debug_print: (a: number) => {
				const str = [];
				let ch;
				while ((ch = this._machineBytes[a++])) str.push(String.fromCharCode(ch));

				console.log(str.join(""));
			}
		}
	};

	get running(): boolean {
		return this._running;
	}

	set running(v: boolean) {
		if (this._running == v) return;

		// The loop is paced by the display: one advance per animation
		// frame, sized by the elapsed time (capped so a long stall —
		// or returning from a hidden tab, where rAF pauses entirely —
		// skips ahead instead of fast-forwarding the machine)
		let time = performance.now();

		const _tick = () => {
			if (!this._running) return;

			const now = performance.now();
			const delta = Math.floor(Math.min(200, now - time) * CPU_FREQ / 1000);

			time = now;

			if (this.breakpoints.size) {
				this.state.clocks += delta;	// advance our clock

				while (this.state.clocks > 0) {
					if (this.breakpoints.has(this.translate(this.state.cpu.pc))) {
						this.running = false;
						break;
					}

					this._exports.cpu_step(this._cpu_state);
				}
			} else {
				this._exports.cpu_advance(this._cpu_state, delta);
			}

			this.update();

			// A breakpoint hit flips running mid-tick; don't reschedule
			if (this._running) {
				this._animation = requestAnimationFrame(_tick);
			}
		};

		this._running = v;

		if (v) {
			// Usually reached from a click (settings toggle), which is a
			// valid gesture to lift the autoplay suspension
			this._audio.resume();
			this._animation = requestAnimationFrame(_tick);
		} else if (this._animation !== undefined) {
			cancelAnimationFrame(this._animation);
			this._animation = undefined;
		}

		this.update();
	}

	// Pushes a finished frame to the display (overridden by the screen panel)
	repaint(_memory: Uint8Array, _address: number): void {

	}

	// Store contract for React's useSyncExternalStore
	subscribe = (listener: () => void): (() => void) => {
		this._listeners.add(listener);
		return () => this._listeners.delete(listener);
	};

	// The state object is a live view over WASM memory, so a monotonic
	// version number stands in for an immutable snapshot
	getVersion = (): number => {
		return this._version;
	};

	// Notify the UI that machine state has changed. Notifications are
	// coalesced to animation frames so the emulation tick (setInterval at
	// 0ms) doesn't force a React render per event-loop turn
	update(): void {
		this._version++;

		if (this._notifyScheduled) return;
		this._notifyScheduled = true;

		requestAnimationFrame(() => {
			this._notifyScheduled = false;
			for (const listener of this._listeners) listener();
		});
	}

	private _updateinput(): void {
		this._exports.update_inputs(this._cpu_state, this._inputState);
	}

	// Cartridge I/O
	load(ab: ArrayBuffer): void {
		const bytes = new Uint8Array(ab);
		const hasHeader = (bytes[0] != 0x50 || bytes[1] != 0x4D);
		const offset = hasHeader ? 0 : 0x2100;

		setTimeout(async () => {
			for (let i = bytes.length - 1; i >= 0; i--) this.state.cartridge[(i + offset) & 0x1FFFFF] = bytes[i];

			// Restore this cartridge's classification, if it has
			// visited before
			this._cartKey = cartridgeKey(this.state.cartridge);
			try {
				const saved = await loadMap(this._cartKey);
				if (saved && saved.length === this.cartridgeTrace.length) {
					this.cartridgeTrace.set(saved);
				}
			} catch (e) {
				console.warn("Could not restore cartridge classification", e);
			}

			this._inputState &= ~INPUT_CART_N;
			this._updateinput();
		}, 100);

		this.eject();
	}

	eject(): void {
		// The classification belongs to the departing cartridge: save
		// it under that cartridge's key, then clear. Snapshot before
		// clearing — the save commits asynchronously.
		if (this._cartKey) {
			void saveMap(this._cartKey, this.cartridgeTrace.slice()).catch(() => {});
			this._cartKey = null;
		}
		this.cartridgeTrace.fill(0);

		this._inputState |= INPUT_CART_N;
		this._updateinput();
	}

	// WASM shim functions
	translate(address: number): number {
		if (address & 0x8000) {
			return (address & 0x7FFF) | (this.state.cpu.cb << 15);
		} else {
			return address;
		}
	}

	step(): void {
		this._exports.cpu_step(this._cpu_state);
		this.update();
	}

	reset(): void {
		this._exports.cpu_reset(this._cpu_state);
		this._updateinput();
		this.update();
	}

	read(address: number): number {
		return this._exports.cpu_read(this._cpu_state, address);
	}

	write(data: number, address: number): void {
		return this._exports.cpu_write(this._cpu_state, data, address);
	}
}
