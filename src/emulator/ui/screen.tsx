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

import React, { Component } from "react";

import VertexShader from "./shaders/vertex.glsl";
import FragmentShader from "./shaders/fragment.glsl";

import classes from "./style.less";

import SystemContext from "../context";

const VRAM_WIDTH  = 96;
const VRAM_HEIGHT = 64;

export default class Registers extends Component {
	static contextType = SystemContext;

	constructor(props) {
		super(props);

		this._ref = React.createRef();
		this._frameIndex = 0;
	}

	componentDidMount() {
		const system = this.context.system;

		this._repainter();
		system.repaint = (memory, address) => {
			const gl = this._ctx;

			if (!gl) return ;

			this._frameIndex = (this._frameIndex + 1) % 4;
			gl.bindTexture(gl.TEXTURE_2D, this._vram);

			gl.texSubImage2D(
				gl.TEXTURE_2D, 0,
				0, this._frameIndex * VRAM_HEIGHT,
				VRAM_WIDTH, VRAM_HEIGHT,
				gl.RGBA_INTEGER, gl.UNSIGNED_BYTE,
				memory, address);
		}
	}

	componentWillUnmount() {
		cancelAnimationFrame(this._af);
		this._ctx = null;

		delete this.context.repaint;
	}

    onDragOver (e) {
        e.preventDefault();
        e.dataTransfer.dropEffect = "copy";
    }

    onDragLeave () {
    }

    onDrop (e) {
        e.preventDefault();

        var file = e.dataTransfer.files[0],
            reader = new FileReader();

        reader.onload = (e) => {
        	this.context.system.load(e.target.result);
        };

        reader.readAsArrayBuffer(file);
    }

	init() {
		const gl = this._ctx;

		gl.disable(gl.STENCIL_TEST);
		gl.disable(gl.DEPTH_TEST);
		gl.disable(gl.DITHER);

		gl.enable(gl.BLEND);
		gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

		gl.colorMask(true, true, true, false);
		gl.clearColor(0xB7 / 255, 0xCA / 255, 0xB7 / 255, 1.0);
		gl.clear(gl.COLOR_BUFFER_BIT);

		this._shader = this._createShader(VertexShader, FragmentShader);

		this._copyBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._copyBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([ 1, 1, 1,-1,-1, 1,-1,-1]), gl.STATIC_DRAW);

		this._vram = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, this._vram);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA8UI, 256, 256, 0, gl.RGBA_INTEGER, gl.UNSIGNED_BYTE, new Uint8Array(0x40000));
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
	}

	_createShader (vertex, fragment) {
		const gl = this._ctx;

		var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
		var vertexShader =  gl.createShader(gl.VERTEX_SHADER);

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

		let shaderProgram = gl.createProgram();
		gl.attachShader(shaderProgram, vertexShader);
		gl.attachShader(shaderProgram, fragmentShader);
		gl.linkProgram(shaderProgram);

		if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
			console.error(gl.getError());
			return null;
		}

		const attrCount = gl.getProgramParameter(shaderProgram, gl.ACTIVE_ATTRIBUTES);
		const attributes = {};

		for (var i = 0; i < attrCount; i++) {
			const attr = gl.getActiveAttrib(shaderProgram, i);
			attributes[attr.name] = i;
		}

		const uniCount= gl.getProgramParameter(shaderProgram, gl.ACTIVE_UNIFORMS);
		const uniforms = {};

		for (var i = 0; i < uniCount; i++) {
			const uni = gl.getActiveUniform(shaderProgram, i);
			uniforms[uni.name] = gl.getUniformLocation(shaderProgram, uni.name);
		}

		return {
			attributes: attributes,
			uniforms: uniforms,
			program: shaderProgram
		};
	}

	_repainter() {
		var time = 0;

		this._af = requestAnimationFrame(() => {
			if (this._previousRef != this._ref.current) {
				this._ctx = this._ref.current.getContext("webgl2", {
					preserveDrawingBuffer: true,
					alpha: false
				});
				this._previousRef = this._ref.current;

				this.init();
			}

			const gl = this._ctx;
			const width = this._ref.current.clientWidth;
			const height = this._ref.current.clientHeight;
			const now = Date.now();
			const delta = (now - this._time) / 1000;

			this._time = now;

			if (width != this._ref.current.width || height != this._ref.current.height) {
				this._ref.current.width = width;
				this._ref.current.height = height;

				if (width * 2 / 3 > height) {
					let fit_x = Math.floor(height * 3 / 2);
					gl.viewport((width - fit_x) / 2, 0, fit_x, height);
				} else {
					let fit_y = width * 2 / 3;
					gl.viewport(0, (height - fit_y) / 2, width, fit_y);
				}


				gl.clear(gl.COLOR_BUFFER_BIT);
			}

			gl.useProgram(this._shader.program);

			gl.activeTexture(gl.TEXTURE0);
			gl.bindTexture(gl.TEXTURE_2D, this._vram);

			gl.uniform1f(this._shader.uniforms.clock, now / 1000 % (24*60*60));
			gl.uniform1f(this._shader.uniforms.time, delta);
			gl.uniform1i(this._shader.uniforms.frame, this._frameIndex);

			gl.uniform1f(this._shader.uniforms.analog, 50);
			gl.uniform1i(this._shader.uniforms.simulate_gray, 1);

			gl.enableVertexAttribArray(this._shader.attributes.vertex);
			gl.bindBuffer(gl.ARRAY_BUFFER, this._copyBuffer);
			gl.vertexAttribPointer(this._shader.attributes.position, 2, gl.FLOAT, false, 0, 0);
			gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

			this._repainter();
		});
	}

	render() {
		return (
			<div onDragOver={(e) => this.onDragOver(e)}
				onDragLeave={(e) => this.onDragLeave(e)}
				onDrop={(e) => this.onDrop(e)}
				className={classes.screen}>
				<canvas ref={this._ref} />
			</div>
		);
	}
}
