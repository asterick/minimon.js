import React, { Component } from "react";
import 'react-virtualized/styles.css';

import MemoryEditor from "./memory";
import Disassembler from "./disassemble";
import Registers from "./registers";
import Screen from "./screen";

import style from "./style.css";

class MinimonDebugger extends Component {
	step() {
		this.props.system.step();
		this.setState({});
	}

	reset() {
		this.props.system.reset();
		this.setState({});
	}

	render() {
		return (
			<div className={style.debugger}>
				<div style={{ width: "576px" }}>
					<Screen system={this.props.system} width={96} height={64} />
				</div>
				<button onClick={() => this.step()}>Step</button>
				<button onClick={() => this.reset()}>Reset</button>
				<Registers system={this.props.system} />
				<Disassembler system={this.props.system} pc={this.props.system.pc} />
			</div>
		);
		
	}
}

export default MinimonDebugger;
