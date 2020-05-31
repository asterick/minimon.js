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
*/

import React from 'react';
import ReactDOM from 'react-dom';
import DockLayout from 'rc-dock'

import Screen from "./screen";
import Registers from "./registers";
import Memory from "./memory";
import Disassembler from "./disassemble";
import Settings from "./settings";

import style from "./style.less";

let screen_tab = { id: 'screen', minWidth: 96, minHeight:64, title: 'Screen', content: <Screen />, closable: false };
let registers_tab = {id: 'registers', title: 'Registers', content: <Registers />, closable: true };
let disassembly_tab = {id: 'disassembly', title: 'Disassembly', content: <Disassembler />, closable: true };
let memory_tab = {id: 'memory', title: <div>'Memory'</div>, content: <Memory baseAddress={0x1000} memory='ram' />, closable: true };
let eeprom_tab = {id: 'eeprom', title: 'EEPROM', content: <Memory baseAddress={0} memory='eeprom' />, closable: true };
let settings_tab = {id: 'settings', title: 'Settings', content: <Settings />, closable: true };

let all_tabs = [
	screen_tab, 
	registers_tab, 
	disassembly_tab, 
	memory_tab, 
	eeprom_tab, 
	settings_tab
];

let defaultLayout = {
	dockbox: {
		mode: 'horizontal',
		children: [
			{
				mode: 'vertical',
				children: [
					{
						tabs: [screen_tab],
					},
					{
						tabs: [settings_tab],
					},
					{
						tabs: [registers_tab],
					}
				]
			},
			{
				mode: 'vertical',
				children: [
					{
						mode: 'vertical',
						children: [
							{ tabs: [disassembly_tab, memory_tab, eeprom_tab] }
						]
					}
				]
				
			}
		]
	}
};

function findTabs(layout) {
	return Object.keys(layout).map((k) => {
		let elem = layout[k];
		if (elem.tabs) {
			return elem.tabs.map((e) => e.id);
		} else if (elem.children) {
			return findTabs(elem.children);
		} else {
		}
	}).flat();
}

export default class UI extends React.Component {
	state = {};

	preserveLayout(layout) {
		window.localStorage.setItem("minimon-layout",JSON.stringify(layout));
	}

	reloadLayout(r) {
		if (this.state.loaded || !r) return ;

		this.setState({loaded:true});

		let layout = window.localStorage.getItem("minimon-layout");
		
		if (!layout) return ;

		layout = JSON.parse(layout);

		/* This will shove tabs not found in arbitrary places */
		let found_tabs = findTabs(layout);
		let add_tabs = [... all_tabs];
		var add = add_tabs.filter((t) => (!t.closable && found_tabs.indexOf(t.id) < 0))

		var top = layout.dockbox;
		while (top.children) {
			top = top.children[0];
		}
		top.tabs.push(... add);

		/* Give the layout to our manager */
		r.loadLayout(layout);
	}

	render() {
		return <DockLayout 
			ref={(r) => this.reloadLayout(r)} 
			defaultLayout={defaultLayout} 
			onLayoutChange={(newLayout) => this.preserveLayout(newLayout)}
			style={{position: 'absolute', left: 0, top: 0, right: 0, bottom: 0}}
			/>;
	}
}
