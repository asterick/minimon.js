import React, { Component } from "react";
import 'react-virtualized/styles.css';

import Memory from "./memory";
import Disassembler from "./disassemble";
import Registers from "./registers";
import Screen from "./screen";
import MenuBar from "./menubar";
import SystemContext from "./context";

import style from "./style.scss";

export default class MinimonDebugger extends Component {
	static contextType = SystemContext;

	render() {
		return <div className={style.mainview}>
			<MenuBar />
			<div className={style.debugger}>
				<div className={style.sidebar}>
					<Screen />
					<Registers registers={this.context.registers} />
					<Memory memoryBottom={0x1000} memoryTop={0x20FF} />
				</div>
				<div className={style.document}>
					<Disassembler target={this.context.translate(this.context.registers.pc)} />
				</div>
			</div>
		</div>
	}
}
