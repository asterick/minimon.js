import React, { Component } from "react";

import style from "./style.scss";

export default class MenuBar extends Component {
	step() {
		this.props.system.step();
		this.forceUpdate();
	}

	reset() {
		this.props.system.reset();
		this.forceUpdate();
	}

	render() {
		return <div className={style.menubar}>
			<button onClick={() => this.step()}>Step</button>
			<button onClick={() => this.reset()}>Reset</button>
		</div>;
	}
}
