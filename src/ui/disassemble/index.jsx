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
import { AutoSizer, List } from 'react-virtualized';

import classes from "./style.less";
import SystemContext from "../context";

import { Disassembler } from "../../system/disassemble.js";

function toHex(v, c) {
	v = v.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

const TRACE_INSTRUCTION   = 0b00000001;
const TRACE_IMMEDIATE     = 0b00000010;
const TRACE_BRANCH_TARGET = 0b00000100;
const TRACE_TILE_DATA     = 0b00001000;
const TRACE_SPRITE_DATA   = 0b00010000;
const TRACE_READ          = 0b01000000;
const TRACE_WRITE         = 0b10000000;

const TERMINATING_INSTRUCTION = ['JRS', 'JRL', 'CARS', 'CARL', 'RETS', 'RET', 'RETE', 'CALL', 'INT', 'JP'];

function type(d) {
	if (d & TRACE_INSTRUCTION) {
		return 'instruction';
	} else if (d & TRACE_TILE_DATA) {
		return 'tile';
	} else if (d & TRACE_SPRITE_DATA) {
		return 'sprite';
	} else {
		return 'data';
	}
}

class MapData extends Component {
	static contextType = SystemContext;

	constructor(props) {
		super(props);
		this.canvas = React.createRef();
	}

	componentDidMount() {
        this.updateCanvas();
	}
	
    componentDidUpdate() {
        this.updateCanvas();
	}

	updateCanvas() {
		const node = this.canvas.current;
		const system = this.context.system;

		if (!node) return ;
		
		const ctx = node.getContext("2d");

		let px = ctx.getImageData(0, 0, this.props.length, 8);
		let ints = new Uint32Array(px.data.buffer);
		let x, y;

		for (x = 0; x < this.props.length; x++) {
			let byte = system.read(this.props.address+x);

			for (y = 0; y < 8; y++) {
				ints[y * this.props.length + x] = (byte & (1 << y)) ? 0xFF000000 : 0xFFCCCCCC;
			}
		}

		ctx.putImageData(px, 0,0);

		console.log(node.clientWidth, node.clientHeight)
	}

	render() {
		this.updateCanvas();
		return <div>
			{toHex(this.props.address, 6)}
			<canvas ref={this.canvas} width={this.props.length} height={8} />
		</div>
	}
}

class SpriteData extends Component {
	static contextType = SystemContext;

	constructor(props) {
		super(props);
		this.canvas = React.createRef();
	}

	componentDidMount() {
        this.updateCanvas();
	}
	
    componentDidUpdate() {
        this.updateCanvas();
	}

	updateCanvas() {
		const node = this.canvas.current;
		const system = this.context.system;

		if (!node) return ;
		
		const ctx = node.getContext("2d");

		let px = ctx.getImageData(0, 0, this.props.length, 8);
		let ints = new Uint32Array(px.data.buffer);
		let x, y;

		for (x = 0; x < this.props.length; x++) {
			let byte = system.read(this.props.address+x);

			for (y = 0; y < 8; y++) {
				ints[y * this.props.length + x] = (byte & (1 << y)) ? 0xFF000000 : 0xFFCCCCCC;
			}
		}

		ctx.putImageData(px, 0,0);

		console.log(node.clientWidth, node.clientHeight)
	}

	render() {
		this.updateCanvas();
		return <div>
			{toHex(this.props.address, 6)}
			<canvas ref={this.canvas} width={this.props.length} height={8} />
		</div>
	}
}

class Disassembly extends Component {
	static contextType = SystemContext;

	constructor(props) {
		super();

		this.dirty = true;
		this.maxAddress = 0x1FFF;

		this.state = {
			trace: new Uint8Array(0x200000),
			address: 0
		};
	}

	trace (cpu, address, kind, data) {
		const trace = this.state.trace;
		let now;

		if (address >= this.maxAddress) {
			this.maxAddress = address;
		}

		// We can no longer assume this data is valid
		if (kind & TRACE_WRITE) {
			now = trace[address] & (TRACE_READ | TRACE_WRITE) | kind;
		} else {
			now = trace[address] | kind;
		}

		if (type(trace[address]) != type(now)) {
			this.dirty = true;
		}

		trace[address] = now;
	}

	componentDidMount() {
		const system = this.context.system;
		system.reload = () => {
			for (let i = 0x2100; i < this.state.trace.length; i++) {
				this.state.trace[i] = 0;
			}
			this.maxAddress = system.romLength;
		}
		system.trace = this.trace.bind(this);
	}

	componentWillUnmount() {
		delete this.context.system.trace;
	}

	_invalidate(target) {
		const child = this.childAt[target];
		if (!child) return ;
		child.invalid = true;
	}

	_rebuild() {
		this.dirty = false;
		let address = 0;

		this.children = [];

		while (address < this.maxAddress) {
			let length = 1;

			let kind = type(this.state.trace[address]);

			switch (kind) {
			default:
			case 'tile': 
				while (type(this.state.trace[address+length]) == 'tile' && length < 256) length++;
				break ;
			case 'data':
				while (type(this.state.trace[address+length]) == 'data' && length < 32) length++;
				break ;
			case 'instruction':
				while (this.state.trace[address+length] & TRACE_IMMEDIATE && ~this.state.trace[address+length] & TRACE_INSTRUCTION) length++;
				break ;
			}

			this.children.push({kind, address, length});
		
			address += length;
		
			if (address >= 0x2000 && address <= 0x20FF) address = 0x2100;
		}
	}

	_renderData(address, length) {
		let bytes = [];
		for (let i = 0; i < length; i++) {
			bytes.push(toHex(this.context.system.read(address+i), 2))
		}

		return `${toHex(address, 6)} ${bytes}`
	}

	_renderInstruction(address, length) {
		let disasm = new Disassembler(this.context.system);
		let inst = disasm.get(address);
		let bytes = [];
		
		if (inst == null) return this._renderData(address, length);

		for (let i = 0; i < length; i++) {
			bytes.push(toHex(this.context.system.read(address+i), 2))
		}

		return `${toHex(address, 6)} ${inst.op} ${inst.args.join(", ")} ${bytes}`
	}

	_rowRenderer({ index, isScrolling, key, parent, style }) {
		const { kind, address, length } = this.children[index];
		let row;

		switch (kind) {
			case 'instruction':
				row = this._renderInstruction(address, length);
				break ;
			case 'tile':
				row = <MapData address={address} length={length} />;
				break ;
			default:
			case 'data':
				row = this._renderData(address, length);
				break ;
		}

		return <div key={key} style={style}>{row}</div>
	}

	render() {
		const system = this.context.system;
		const { target } = this.props;
		let pc = system.translate(system.state.cpu.pc);

		/* Auto trace any PC location that has not already been */
		if (type(this.state.trace[pc]) != 'instruction') {
			let disasm = new Disassembler(this.context.system);
			this.dirty = true;

			for(;;) {
				const op = disasm.get(pc);
				
				if (!op) break ;
				
				let opc = pc;
				this.state.trace[pc++] |= TRACE_INSTRUCTION | TRACE_IMMEDIATE;
				for (let i = 1; i < op.data.length; i++) {
					this.state.trace[pc++] |= TRACE_IMMEDIATE;
				}

				if (TERMINATING_INSTRUCTION.indexOf(op.op) >= 0) break ;
				if ((opc & ~0x7FFF) != (pc & ~0x7FFF)) break ;
			}
		}

		/* Rebuild broken list if changes to view hve occured */
		if (this.dirty) {
			this._rebuild();
		}

		let followPC;
		if (false) {
			for (let row = 0; row < this.children.length; row++) {
				let { address, length } = this.children[row];
				if (pc >= address && pc < address + length) {
					followPC = row;
					break ;
				}
			}
		}

		//console.log(followPC);

		return (
			<AutoSizer>
				{({ width, height }) => {
					return <List
						width={width}
						height={height}
						scrollToIndex={followPC}
						rowHeight={22}
						rowRenderer={this._rowRenderer.bind(this)}
						rowCount={this.children.length}
						/>
				}}
			</AutoSizer>
		);
	}
}

export default Disassembly;
