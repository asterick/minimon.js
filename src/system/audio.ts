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

/*
 * The processor runs on the audio rendering thread: it keeps a ring
 * buffer fed by postMessage from the main thread and decays the last
 * sample towards silence on underrun (same behaviour as the old
 * ScriptProcessorNode implementation).
 *
 * It is shipped as a Blob URL rather than a separate module so the
 * code needs no bundler-specific worklet handling and behaves the
 * same under the dev server and `vite build`.
 */
const PROCESSOR_SOURCE = `
class MinimonAudioProcessor extends AudioWorkletProcessor {
	constructor() {
		super();

		this.buffer = new Float32Array(4096);
		this.writeIndex = 0;
		this.readIndex = 0;
		this.sample = 0.0;

		this.port.onmessage = (e) => {
			const f = e.data;

			for (let i = 0; i < f.length; i++) {
				this.buffer[this.writeIndex++] = f[i];
				if (this.writeIndex >= this.buffer.length) this.writeIndex = 0;
			}
		};
	}

	process(inputs, outputs) {
		const audio = outputs[0][0];

		for (let i = 0; i < audio.length; i++) {
			if (this.readIndex != this.writeIndex) {
				this.sample = this.buffer[this.readIndex++] * 0.1;
				if (this.readIndex >= this.buffer.length) this.readIndex = 0;
			} else {
				this.sample *= 0.95;
			}
			audio[i] = this.sample;
		}

		return true;
	}
}

registerProcessor('minimon-audio', MinimonAudioProcessor);
`;

export default class Audio {
	context: AudioContext;

	private _node: AudioWorkletNode | null = null;

	constructor() {
		this.context = new AudioContext();

		void this._init();

		// Autoplay policy: an AudioContext created without a user gesture
		// starts (and stays) suspended. Resume it on the first interaction.
		document.addEventListener("pointerdown", this._resume);
		document.addEventListener("keydown", this._resume);
		this.context.addEventListener("statechange", () => {
			if (this.context.state === "running") {
				document.removeEventListener("pointerdown", this._resume);
				document.removeEventListener("keydown", this._resume);
			}
		});
	}

	private async _init(): Promise<void> {
		const url = URL.createObjectURL(new Blob([PROCESSOR_SOURCE], { type: "application/javascript" }));

		try {
			await this.context.audioWorklet.addModule(url);
		} finally {
			URL.revokeObjectURL(url);
		}

		this._node = new AudioWorkletNode(this.context, 'minimon-audio', {
			numberOfInputs: 0,
			numberOfOutputs: 1,
			outputChannelCount: [1]
		});
		this._node.connect(this.context.destination);
	}

	private _resume = () => {
		this.resume();
	};

	resume(): void {
		if (this.context.state === "suspended") {
			void this.context.resume();
		}
	}

	get sampleRate(): number {
		return this.context.sampleRate;
	}

	push(f: Float32Array): void {
		// The view aliases WASM memory that the core will overwrite, so
		// copy the samples out; the copy is transferred, not re-copied.
		// Pushes before the worklet module resolves are dropped.
		if (!this._node) return;

		const copy = f.slice();
		this._node.port.postMessage(copy, [copy.buffer]);
	}
}
