import React, { Component } from "react";
import 'react-virtualized/styles.css';

import MemoryEditor from "./memory";
import Disassembler from "./disassemble";

import style from "./style.css";

class MinimonDebugger extends Component {
	render() {
		return (
			<div className={style.debugger}>
				<Disassembler system={this.props.system} />
				
			</div>
		);
		//<MemoryEditor system={this.props.system}/>
	}
}

export default MinimonDebugger;
