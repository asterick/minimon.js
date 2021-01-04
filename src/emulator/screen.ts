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

import VertexShader from "raw-loader!./shaders/vertex.glsl";
import FragmentShader from "raw-loader!./shaders/fragment.glsl";

const VRAM_WIDTH  = 96;
const VRAM_HEIGHT = 64;

interface UniformList {
	[key: string]: WebGLUniformLocation;
}

interface AttributeList {
	[key: string]: number;
}

interface Shader {
	program: WebGLShader;
	uniforms: UniformList;
	attributes: AttributeList;
}

let element: HTMLCanvasElement;

let _frameIndex: number;
let _time: number;

let	gl: WebGL2RenderingContext;
let _vram: WebGLTexture;
let _copyBuffer: WebGLBuffer;
let _shader: Shader;

export function init(): void {
	/* Create our dom element */
	element = document.createElement("canvas");
	element.width = VRAM_WIDTH;
	element.height = VRAM_HEIGHT;
	document.body.appendChild(element);

	_frameIndex = 0;

	/* Configure our GL context */
	gl = element.getContext("webgl2", {
		preserveDrawingBuffer: true,
		alpha: false
	});

	gl.disable(gl.STENCIL_TEST);
	gl.disable(gl.DEPTH_TEST);
	gl.disable(gl.DITHER);

	gl.enable(gl.BLEND);
	gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

	gl.colorMask(true, true, true, false);
	gl.clearColor(0xB7 / 255, 0xCA / 255, 0xB7 / 255, 1.0);
	gl.clear(gl.COLOR_BUFFER_BIT);

	_shader = _createShader(VertexShader, FragmentShader);

	_copyBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, _copyBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([ 1, 1, 1,-1,-1, 1,-1,-1]), gl.STATIC_DRAW);

	_vram = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, _vram);
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA8UI, 256, 256, 0, gl.RGBA_INTEGER, gl.UNSIGNED_BYTE, new Uint8Array(0x40000));
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

	/* Kick off our render loop */
	_repainter();
}

function _createShader (vertex: string, fragment: string): Shader {
	const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
	const vertexShader =  gl.createShader(gl.VERTEX_SHADER);

	gl.shaderSource(vertexShader, vertex);
	gl.compileShader(vertexShader);

	if (!gl.getShaderParameter(vertexShader, gl.COMPILE_STATUS)) {
		console.error(gl.getShaderInfoLog(vertexShader));
		return null;
	}

	gl.shaderSource(fragmentShader, fragment);
	gl.compileShader(fragmentShader);

	if (!gl.getShaderParameter(fragmentShader, gl.COMPILE_STATUS)) {
		console.error(gl.getShaderInfoLog(fragmentShader));
		return null;
	}

	const shaderProgram = gl.createProgram();
	gl.attachShader(shaderProgram, vertexShader);
	gl.attachShader(shaderProgram, fragmentShader);
	gl.linkProgram(shaderProgram);

	if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
		console.error(gl.getError());
		return null;
	}

	const attrCount = gl.getProgramParameter(shaderProgram, gl.ACTIVE_ATTRIBUTES);
	const attributes:AttributeList = {};

	for (let i = 0; i < attrCount; i++) {
		const attr = gl.getActiveAttrib(shaderProgram, i);
		attributes[attr.name] = i;
	}

	const uniCount= gl.getProgramParameter(shaderProgram, gl.ACTIVE_UNIFORMS);
	const uniforms:UniformList = {};

	for (let i = 0; i < uniCount; i++) {
		const uni = gl.getActiveUniform(shaderProgram, i);
		uniforms[uni.name] = gl.getUniformLocation(shaderProgram, uni.name);
	}

	return {
		attributes: attributes,
		uniforms: uniforms,
		program: shaderProgram
	};
}

function _repainter() {
	requestAnimationFrame(() => {
		const width = Math.max(element.clientWidth, VRAM_WIDTH);
		const height = Math.max(element.clientHeight, VRAM_HEIGHT);
		const now = Date.now();
		const delta = (now - _time) / 1000;

		_time = now;

		if (width != element.width || height != element.height) {
			element.width = width;
			element.height = height;

			if (width * 2 / 3 > height) {
				const fit_x = Math.floor(height * 3 / 2);
				gl.viewport((width - fit_x) / 2, 0, fit_x, height);
			} else {
				const fit_y = width * 2 / 3;
				gl.viewport(0, (height - fit_y) / 2, width, fit_y);
			}


			gl.clear(gl.COLOR_BUFFER_BIT);
		}

		gl.useProgram(_shader.program);

		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(gl.TEXTURE_2D, _vram);

		gl.uniform1f(_shader.uniforms.clock, now / 1000 % (24*60*60));
		gl.uniform1f(_shader.uniforms.time, delta);
		gl.uniform1i(_shader.uniforms.frame, _frameIndex);

		gl.uniform1f(_shader.uniforms.analog, 50);
		gl.uniform1i(_shader.uniforms.simulate_gray, 1);

		gl.enableVertexAttribArray(_shader.attributes.vertex);
		gl.bindBuffer(gl.ARRAY_BUFFER, _copyBuffer);
		gl.vertexAttribPointer(_shader.attributes.position, 2, gl.FLOAT, false, 0, 0);
		gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

		_repainter();
	});
}

export function repaint (memory: Uint8Array, address: number): void {
	_frameIndex = (_frameIndex + 1) % 4;
	gl.bindTexture(gl.TEXTURE_2D, _vram);

	gl.texSubImage2D(
		gl.TEXTURE_2D, 0,
		0, _frameIndex * VRAM_HEIGHT,
		VRAM_WIDTH, VRAM_HEIGHT,
		gl.RGBA_INTEGER, gl.UNSIGNED_BYTE,
		memory, address);
}
