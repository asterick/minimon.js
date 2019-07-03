import React, { Component } from "react";
import 'react-virtualized/styles.css';

import Memory from "./memory";
import Disassembler from "./disassemble";
import Registers from "./registers";
import Screen from "./screen";

import style from "./style.scss";

class MinimonDebugger extends Component {
	constructor(props) {
		super(props);

		setInterval(() => this.step(), 10)
	}

	step() {
		this.props.system.step();
		this.setState({});
	}

	reset() {
		this.props.system.reset();
		this.setState({});
	}

	render() {
		return <div className={style.mainview}>
			<div className={style.menubar}>
				<button onClick={() => this.step()}>Step</button>
				<button onClick={() => this.reset()}>Reset</button>
			</div>
			<div className={style.debugger}>
				<div className={style.sidebar}>
					<Screen system={this.props.system} />
					<Registers registers={this.props.system.registers} />
					<Memory memoryBottom={0x1000} memoryTop={0x2000} system={this.props.system} />
				</div>
				<div className={style.document}>
					<Disassembler follow_pc={true} system={this.props.system} target={this.props.system.translate(this.props.system.registers.pc)} />
				</div>
			</div>
		</div>
	}
}

export default MinimonDebugger;
