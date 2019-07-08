import React, { Component } from "react";
import 'react-virtualized/styles.css';

import Memory from "./memory";
import Disassembler from "./disassemble";
import Registers from "./registers";
import Screen from "./screen";
import MenuBar from "./menubar";

import style from "./style.scss";

export default class MinimonDebugger extends Component {
	render() {
		return <div className={style.mainview}>
			<MenuBar system={this.props.system} />
			<div className={style.debugger}>
				<div className={style.sidebar}>
					<Screen system={this.props.system} />
					<Registers registers={this.props.system.registers} />
					<Memory memoryBottom={0x1000} memoryTop={0x20FF} system={this.props.system} />
				</div>
				<div className={style.document}>
					<Disassembler system={this.props.system} target={this.props.system.translate(this.props.system.registers.pc)} />
				</div>
			</div>
		</div>
	}
}
