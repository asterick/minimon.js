import React, { Component } from "react";

import classes from "./style.css";

function toHex(i, c) {
	let v = i.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

class Register extends Component {
	
}

class Registers extends Component {
	render() {
		const sc = this.props.registers.sc;

		return (
			<div>
				<table>
					<tbody>
						<tr><td>BA</td><td>{toHex(this.props.registers.ba, 4)}</td><td>BR</td><td>{toHex(this.props.registers.br, 2)}</td><td>I</td><td>{sc >> 6}</td></tr>
						<tr><td>HL</td><td>{toHex(this.props.registers.hl, 4)}</td><td>EP</td><td>{toHex(this.props.registers.ep, 2)}</td><td>U</td><td>{(sc & 0x20) ? '\u2713' : '\u2015'}</td></tr>
						<tr><td>IX</td><td>{toHex(this.props.registers.ix, 4)}</td><td>XP</td><td>{toHex(this.props.registers.xp, 2)}</td><td>D</td><td>{(sc & 0x10) ? '\u2713' : '\u2015'}</td></tr>
						<tr><td>IY</td><td>{toHex(this.props.registers.iy, 4)}</td><td>YP</td><td>{toHex(this.props.registers.yp, 2)}</td><td>N</td><td>{(sc & 0x08) ? '\u2713' : '\u2015'}</td></tr>
						<tr><td>PC</td><td>{toHex(this.props.registers.pc, 4)}</td><td>CB</td><td>{toHex(this.props.registers.cb, 2)}</td><td>V</td><td>{(sc & 0x04) ? '\u2713' : '\u2015'}</td></tr>
						<tr><td>SP</td><td>{toHex(this.props.registers.sp, 4)}</td><td>NB</td><td>{toHex(this.props.registers.nb, 2)}</td><td>C</td><td>{(sc & 0x02) ? '\u2713' : '\u2015'}</td></tr>
						<tr><td /><td /><td>SC</td><td>{toHex(sc, 2)}</td><td>Z</td><td>{(sc & 0x01) ? '\u2713' : '\u2015'}</td></tr>
					</tbody>
				</table>
			</div>
		);
	}
}

export default Registers;
