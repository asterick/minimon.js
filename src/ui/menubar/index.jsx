import React, { Component } from "react";

import style from "./style.scss";

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
	step() {
		this.props.system.step();
		this.parent.forceUpdate();
	}

	reset() {
		this.props.system.reset();
		this.forceUpdate();
	}

	render() {
		return <div className={style.menubar}>
			<DropDown title="System">
				<MenuItem title="step" onClick={() => this.step()} />
				<MenuItem title="reset" onClick={() => this.reset()} />
				<Seperator />
				<DropDown title="text">
					<MenuItem title="step" onClick={() => this.step()} />
					<MenuItem title="reset" onClick={() => this.reset()} />
				</DropDown>
			</DropDown>
			<DropDown title="System">
				<MenuItem title="step" onClick={() => this.step()} />
				<MenuItem title="reset" onClick={() => this.reset()} />
				<DropDown title="text">
					<MenuItem title="step" onClick={() => this.step()} />
					<MenuItem title="reset" onClick={() => this.reset()} />
				</DropDown>
			</DropDown>
			<DropDown title="System">
				<MenuItem title="step" onClick={() => this.step()} />
				<MenuItem title="reset" onClick={() => this.reset()} />
				<DropDown title="text">
					<MenuItem title="step" onClick={() => this.step()} />
					<MenuItem title="reset" onClick={() => this.reset()} />
				</DropDown>
			</DropDown>
		</div>;
	}
}
