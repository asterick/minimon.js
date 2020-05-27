import style from "./style.less";
/*
ISC License

Copyright (c) 2019, Bryon Vandiver

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

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

	constructor(props) {
		super(props);

		this.state = {
			fullscreen: false
		};
	}

		onDragOver (e) {
				e.preventDefault();
				e.dataTransfer.dropEffect = "copy";
				this.setState({ dragging: true });
		}

		onDragLeave () {
				this.setState({ dragging: false });
		}

	componentDidMount() {
		this.context.update = () => this.forceUpdate();
	}

	componentWillUnmount() {
		delete this.context.update;
	}

		onDrop (e) {
				e.preventDefault();

				var file = e.dataTransfer.files[0],
						reader = new FileReader();

				reader.onload = (e) => {
					this.context.load(e.target.result);
				};

				this.setState({ dragging: false }, function () {
						reader.readAsArrayBuffer(file);
				});
		}

		innerUI() {
			if (this.state.fullscreen) {
				return <div className={style.debugger}>
					<div className={`${style.screen} ${style.fullsize}`}>
					<Screen />
				</div>
			</div>
			} else {
			const system = this.context;

				return <div className={style.debugger}>
					<div className={style.sidebar}>
					<div className={style.screen}>
						<Screen />
					</div>
					<Registers />
					<Memory baseAddress={0x1000} memory={system.state.ram} />
				</div>
				<div className={style.document}>
					<div className={style.viewer}>
						<Disassembler target={system.state.cpu.pc} />
					</div>
					<div className={style.viewer}>
						<Memory baseAddress={0} memory={system.state.gpio.eeprom.data} />
					</div>
				</div>
			</div>;
			}
		}

	render() {
		return <div 
						onDragOver={(e) => this.onDragOver(e)}
						onDragLeave={(e) => this.onDragLeave(e)}
						onDrop={(e) => this.onDrop(e)}
			className={style.mainview}>

			<MenuBar />
			
			{this.innerUI()}
		</div>
	}
}
*/


import React from 'react';
import ReactDOM from 'react-dom';
import DockLayout from 'rc-dock'

let tab1 = {id: 't1', title: 'Tab 1', content: <div>Tab 1</div>};
let tab2 = {id: 't2', title: 'Tab 2', content: <div>Tab 2</div>};
let tab3 = {id: 't3', title: 'Tab 3', content: <div>Tab 3</div>};
let tab4 = {id: 't4', title: 'Tab 4', content: <div>Tab 4</div>};
let tab5 = {id: 't5', title: 'Tab 5', content: <div>Tab 5</div>};
let tab6 = {id: 't6', title: 'Tab 6', content: <div>Tab 6</div>};

let defaultLayout = {
	dockbox: {
		mode: 'horizontal',
		children: [
			{
				mode: 'vertical',
				children: [
					{
						tabs: [tab1],
					},
					{
						tabs: [tab2, tab3, tab4],
					}
				]
			},
			{
				tabs: [tab5, tab6],
			},
		]
	}
};
let panelLayout = {
	dockbox: {
		mode: 'horizontal',
		children: [
			{
				tabs: [{id: 't1'}, {id: 't2'}, {id: 't3'}, {id: 't4'}, {id: 't5'}, {id: 't6'}, {id: 'jsxTab'}, {id: 'htmlTab'}],
			},
		]
	}
};
let horizontalLayout = {
	dockbox: {
		mode: 'horizontal',
		children: [
			{tabs: [{id: 't1'}, {id: 'jsxTab'}, {id: 'htmlTab'}]},
			{tabs: [{id: 't2'}]},
			{tabs: [{id: 't3'}]},
			{tabs: [{id: 't4'}]},
			{tabs: [{id: 't5'}]},
			{tabs: [{id: 't6'}]},
		]
	}
};

export default class UI extends React.Component {
	getRef = (r) => {
		this.dockLayout = r;
	};

	state = {saved: null};

	render() {
		console.log(this.state.saved)

		return (
			<div>
				<DockLayout ref={this.getRef} defaultLayout={defaultLayout}
										style={{position: 'absolute', left: 0, top: 0, right: 0, bottom: 0}}/>
				<div className='top-panel'>
					Save Layout:
					<button style={{marginRight: 20}}
									onClick={() => this.setState({saved: this.dockLayout.saveLayout()})}>
						Save
					</button>
					Load Layout:
					<button onClick={() => this.dockLayout.loadLayout(horizontalLayout)}>
						Horizontal
					</button>
					<button onClick={() => this.dockLayout.loadLayout(panelLayout)}>
						Single Panel
					</button>
					<button disabled={this.state.saved == null} onClick={() => this.dockLayout.loadLayout(this.state.saved)}>
						Saved Layout
					</button>
				</div>
			</div>
		);
	}
}
