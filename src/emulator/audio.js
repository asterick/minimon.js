/*
 * MIT License
 *
 * Copyright (c) 2019-2021, Bryon Vandiver
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
