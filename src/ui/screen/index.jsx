import React, { Component } from "react";

import VertexShader from "./shaders/vertex.glsl";
import FragmentShader from "./shaders/fragment.glsl";

import classes from "./style.css";

const DATA_WORDS = 96 * 64 / 8;

class Registers extends Component {
	constructor(props) {
		super(props);

		this._memory = new Uint8Array(DATA_WORDS);
		this._ref = React.createRef();
	}

	componentDidMount() {
		this._repainter();
	}

	componentWillUnmount() {
		cancelAnimationFrame(this._af);
		this._ctx = null;
	}

	init() {
		const gl = this._ctx;

		gl.disable(gl.STENCIL_TEST);
		gl.disable(gl.DEPTH_TEST);
		gl.disable(gl.DITHER);
		gl.colorMask(true, true, true, true);
		gl.clearColor(0, 0, 0, 1);

		gl.clear(gl.COLOR_BUFFER_BIT);

		this._shader = this._createShader(VertexShader, FragmentShader);

		this._copyBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._copyBuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([ 1, 1, 1,-1,-1, 1,-1,-1]), gl.STATIC_DRAW);
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
		let time = 0;

		this._af = requestAnimationFrame(() => {
			if (this._previousRef != this._ref.current) {
				this._ctx = this._ref.current.getContext("webgl2");
				this._previousRef = this._ref.current;
			
				this.init();
			}

			const gl = this._ctx;
			const width = this._ref.current.clientWidth;
			const height = this._ref.current.clientHeight;
			const now = Date.now()

			this._ref.current.width = width;
			this._ref.current.height = height;

			if (!this.g) {
				gl.viewport(0, 0, width, height);
				this.g = true;
			}

			for (let i = 0; i < this._memory.length; i++) {
				this._memory[i] = Math.random() * 0xFF;
			}

			// Select vram as our source texture
			gl.enableVertexAttribArray(this._shader.attributes.vertex);
			gl.useProgram(this._shader.program);

			gl.bindBuffer(gl.ARRAY_BUFFER, this._copyBuffer);
			gl.vertexAttribPointer(this._shader.attributes.aVertex, 2, gl.FLOAT, false, 0, 0);

			gl.uniform1fv(this._shader.uniforms.memory, this._memory);
			gl.uniform1f(this._shader.uniforms.time, Math.min(1, (now - time) / 1000));
			time = now;

			gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

			this._repainter();
		});
	}

	render() {
		return (
			<div className={classes.screen}>
				<div className={classes.container}>
					<canvas ref={this._ref} />
				</div>
			</div>
		);
	}
}

export default Registers;
