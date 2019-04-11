import React, { Component } from "react";
import MemoryEditor from "./memory";

import 'react-virtualized/styles.css';
import style from "./style.css";

class MinimonDebugger extends Component {
	render() {
		return (
			<div className={style.debugger}>
				<MemoryEditor system={this.props.system}/>
			</div>
		);
	}
}

export default MinimonDebugger;
