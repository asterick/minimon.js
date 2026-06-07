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

import { useEffect, useRef } from "react";

import VertexShader from "./shaders/vertex.glsl?raw";
import FragmentShader from "./shaders/fragment.glsl?raw";

import classes from "./style.module.less";

import { useSystem } from "../context";

const VRAM_WIDTH  = 96;
const VRAM_HEIGHT = 64;

interface Shader {
	attributes: Record<string, number>;
	uniforms: Record<string, WebGLUniformLocation | null>;
	program: WebGLProgram;
}

function createShader(gl: WebGL2RenderingContext, vertex: string, fragment: string): Shader | null {
	const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER)!;
	const vertexShader = gl.createShader(gl.VERTEX_SHADER)!;

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

	const shaderProgram = gl.createProgram()!;
	gl.attachShader(shaderProgram, vertexShader);
	gl.attachShader(shaderProgram, fragmentShader);
	gl.linkProgram(shaderProgram);

	if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
		console.error(gl.getError());
		return null;
	}

	const attrCount = gl.getProgramParameter(shaderProgram, gl.ACTIVE_ATTRIBUTES);
	const attributes: Record<string, number> = {};

	for (let i = 0; i < attrCount; i++) {
		const attr = gl.getActiveAttrib(shaderProgram, i)!;
		attributes[attr.name] = i;
	}

	const uniCount = gl.getProgramParameter(shaderProgram, gl.ACTIVE_UNIFORMS);
	const uniforms: Record<string, WebGLUniformLocation | null> = {};

	for (let i = 0; i < uniCount; i++) {
		const uni = gl.getActiveUniform(shaderProgram, i)!;
		uniforms[uni.name] = gl.getUniformLocation(shaderProgram, uni.name);
	}

	return {
		attributes: attributes,
		uniforms: uniforms,
		program: shaderProgram
	};
}

export default function Screen() {
	const system = useSystem();
	const canvasRef = useRef<HTMLCanvasElement>(null);

	useEffect(() => {
		const canvas = canvasRef.current;
		if (!canvas) return;

		const gl = canvas.getContext("webgl2", {
			preserveDrawingBuffer: true,
			alpha: false
		});
		if (!gl) return;

		gl.disable(gl.STENCIL_TEST);
		gl.disable(gl.DEPTH_TEST);
		gl.disable(gl.DITHER);

		gl.enable(gl.BLEND);
		gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

		gl.colorMask(true, true, true, false);
		gl.clearColor(0xB7 / 255, 0xCA / 255, 0xB7 / 255, 1.0);
		gl.clear(gl.COLOR_BUFFER_BIT);

		const shader = createShader(gl, VertexShader, FragmentShader);
		if (!shader) return;

		const copyBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, copyBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([ 1, 1, 1,-1,-1, 1,-1,-1]), gl.STATIC_DRAW);

		const vram = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, vram);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA8UI, 256, 256, 0, gl.RGBA_INTEGER, gl.UNSIGNED_BYTE, new Uint8Array(0x40000));
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

		// The core pushes finished frames here as they are rendered
		let frameIndex = 0;
		system.repaint = (memory, address) => {
			frameIndex = (frameIndex + 1) % 4;
			gl.bindTexture(gl.TEXTURE_2D, vram);

			gl.texSubImage2D(
				gl.TEXTURE_2D, 0,
				0, frameIndex * VRAM_HEIGHT,
				VRAM_WIDTH, VRAM_HEIGHT,
				gl.RGBA_INTEGER, gl.UNSIGNED_BYTE,
				memory, address);
		};

		let af = 0;
		let time = Date.now();

		const repaint = () => {
			af = requestAnimationFrame(() => {
				const width = canvas.clientWidth;
				const height = canvas.clientHeight;
				const now = Date.now();
				const delta = (now - time) / 1000;

				time = now;

				if (width != canvas.width || height != canvas.height) {
					canvas.width = width;
					canvas.height = height;

					if (width * 2 / 3 > height) {
						const fit_x = Math.floor(height * 3 / 2);
						gl.viewport((width - fit_x) / 2, 0, fit_x, height);
					} else {
						const fit_y = width * 2 / 3;
						gl.viewport(0, (height - fit_y) / 2, width, fit_y);
					}

					gl.clear(gl.COLOR_BUFFER_BIT);
				}

				gl.useProgram(shader.program);

				gl.activeTexture(gl.TEXTURE0);
				gl.bindTexture(gl.TEXTURE_2D, vram);

				gl.uniform1f(shader.uniforms.clock, now / 1000 % (24*60*60));
				gl.uniform1f(shader.uniforms.time, delta);
				gl.uniform1i(shader.uniforms.frame, frameIndex);

				gl.uniform1f(shader.uniforms.analog, 50);
				gl.uniform1i(shader.uniforms.simulate_gray, 1);

				gl.enableVertexAttribArray(shader.attributes.vertex);
				gl.bindBuffer(gl.ARRAY_BUFFER, copyBuffer);
				gl.vertexAttribPointer(shader.attributes.vertex, 2, gl.FLOAT, false, 0, 0);
				gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

				repaint();
			});
		};

		repaint();

		return () => {
			cancelAnimationFrame(af);
			system.repaint = () => {};
		};
	}, [system]);

	function onDragOver(e: React.DragEvent) {
		e.preventDefault();
		e.dataTransfer.dropEffect = "copy";
	}

	function onDrop(e: React.DragEvent) {
		e.preventDefault();

		const file = e.dataTransfer.files[0];
		if (!file) return;

		file.arrayBuffer().then((ab) => system.load(ab));
	}

	return (
		<div onDragOver={onDragOver}
			onDrop={onDrop}
			className={classes.screen}>
			<canvas ref={canvasRef} />
		</div>
	);
}
