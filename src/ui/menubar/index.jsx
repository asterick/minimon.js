import React, { Component } from "react";

import style from "./style.scss";
import SystemContext from "../context";

const MenuItem = (props) => {
	const { title } = props;
	return <span className={style.menuitem} {... props}>{title}</span>
}

const DropDown = ({ title, children }) => 
	<div className={style.dropdown}>
		<span className={style.menuitem}>{title}</span>
		<div className={style.dropchildren}>{ children }</div>
	</div>;

const Seperator = () => <div className={style.seperator}></div>

export default class MenuBar extends Component {
	static contextType = SystemContext;

	step() {
		const system = this.context;

		system.step();
	}

	reset() {
		const system = this.context;

		system.reset();
	}

	render() {
		return <div className={style.menubar}>
			<DropDown title="System">
				<MenuItem title={this.context.running ? "stop" : "run"} onClick={() => (this.context.running = !this.context.running)} />
				<Seperator />
				<MenuItem title="step" onClick={() => this.step()} />
				<MenuItem title="reset" onClick={() => this.reset()} />
			</DropDown>
		</div>
	}
}
