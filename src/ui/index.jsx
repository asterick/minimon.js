import React, { Component } from "react";
import 'react-virtualized/styles.css';

import MemoryEditor from "./memory";
import Disassembler from "./disassemble";
import Registers from "./registers";

import style from "./style.css";

class MinimonDebugger extends Component {
	render() {
		return (
			<div className={style.debugger}>
				<MemoryEditor bytesPerRow={0x20} system={this.props.system} />
				
			</div>
		);
		
	}
}

export default MinimonDebugger;
