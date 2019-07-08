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
		const system = this.context;

		return <div className={style.mainview}>
			<MenuBar repaint={() => this.setState({})} />
			<div className={style.debugger}>
				<div className={style.sidebar}>
					<Screen />
					<Registers />
					<Memory memoryBottom={0x1000} memoryTop={0x20FF} />
				</div>
				<div className={style.document}>
					<Disassembler target={system.translate(system.registers.pc)} />
				</div>
			</div>
		</div>
	}
}
