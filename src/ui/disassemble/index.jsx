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
import { AutoSizer } from 'react-virtualized';

import classes from "./style.less";
import SystemContext from "../context";

import { Disassembler } from "../../system/disassemble.js";

function toHex(v, c) {
	v = v.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

class Disassembly extends Component {
	static contextType = SystemContext;

	constructor(props) {
		super(props);

		this.state = {
			address: 0
		};
	}

	renderArg() {
		return [];
	}

	componentDidUpdate(props) {
		if (!this.props.follow_pc) {
			return ;
		}

		const system = this.context.system;
		const target = system.state.cpu.pc;
		const disasm = new Disassembler(system);

		const lines = disasm.process(this.state.address, 21);

		let address = target;

		for (let i = 0; i < lines.length; i++) {
			if (lines[i].address == target) {
				address = lines[Math.max(0, i - 21 / 2) | 0].address;
			}
		}

		if (address !== this.state.address) {
			this.setState({ address });
		}
	}

	onClick(address) {
		const system = this.context.system;

		if (system.breakpoints.indexOf(address) >= 0) {
			system.breakpoints = system.breakpoints.filter((v) => v != address);
		} else {
			system.breakpoints.push(address);
		}

		this.setState({})
	}

	render() {
		const { target } = this.props;

		return (
			<AutoSizer className={classes.disasm}>
				{({ height, width }) => {
					const rowCount = Math.floor(height / 20);

					const system = this.context.system;
					const disasm = new Disassembler(system);
					const lines = disasm.process(this.state.address, rowCount);

					return <table style={{width: `${width}px`, height: `${height}px`}}>
						<tbody>
							{
							lines.map(line =>
								<tr onClick={() => this.onClick(line.address)} key={line.address} className={(target == line.address) ? classes['active'] : ''}>
									<td className={classes["address"]}>{toHex(line.address, 6)}</td>
									<td>{line.data.map((v, i) => <span className={classes['byte-cell']} key={i}>{toHex(v, 2)}</span>)}</td>
									<td className={system.breakpoints && system.breakpoints.indexOf(line.address) >= 0 ? classes['breakpoint'] : ''}>{line.op}</td>
									<td>{line.args.map((s, i) => <span key={i}>{s}</span>)}</td>
									<td>{line.comment}</td>
								</tr>)
							}
						</tbody>
					</table>
				}}
			</AutoSizer>
		);
	}
}

Disassembly.defaultProps = {
	follow_pc: true
};

export default Disassembly;
