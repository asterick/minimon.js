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

const BUFFER_LENGTH = 1024; // This is the size of an audio clip push

export default class Audio {
	constructor() {
		this.context = new AudioContext();

        this.node = this.context.createScriptProcessor(BUFFER_LENGTH, 1, 1);
        this.node.onaudioprocess = this.process.bind(this);

        this._buffer = new Float32Array(BUFFER_LENGTH * 4);
        this._writeIndex = 0;
        this._readIndex = 0;
        this._sample = 0.0;

		this.node.connect(this.context.destination);
	}

    get sampleRate() {
        return this.context.sampleRate;
    }

    push(f) {
        for (let i = 0; i < f.length; i++) {
            this._buffer[this._writeIndex++] = f[i];
            if (this._writeIndex >= this._buffer.length) this._writeIndex = 0;
        }
    }

    process(e) {
        var audio = e.outputBuffer.getChannelData(0),
            length = audio.length;

        for(let i = 0; i < length; i++) {
            if (this._readIndex != this._writeIndex) {
                this._sample = this._buffer[this._readIndex++] * 0.1;
                if (this._readIndex >= this._buffer.length) this._readIndex = 0;
            } else { 
                this._sample *= 0.95;
            }
            audio[i] = this._sample;
        }
    }
}
