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
import Blitter from "./blitter";

import { IconButton } from '@fluentui/react/lib/Button';
import style from "./style.less";

let screen_tab = { 
	id: 'screen',
	minWidth: 96, 
	minHeight: 64,
	title: 'Screen', 
	content: <Screen />, 
	closable: false 
};
let settings_tab = {id: 'settings', title: 'Settings', content: <Settings />, closable: false };

let blitter_tab = {id: 'blitter', title: 'Blitter', content: <Blitter />, closable: true };
let registers_tab = {id: 'registers', title: 'Registers', content: <Registers />, closable: true };
let disassembly_tab = {id: 'disassembly', title: 'Disassembly', content: <Disassembler />, closable: true };
let memory_tab = {id: 'memory', title: 'Memory', content: <Memory baseAddress={0x1000} memory='ram' />, closable: false };
let eeprom_tab = {id: 'eeprom', title: 'EEPROM', content: <Memory baseAddress={0} memory='eeprom' />, closable: true };

let all_tabs = [
	screen_tab, 
	registers_tab, 
	disassembly_tab, 
	memory_tab, 
	eeprom_tab, 
	settings_tab,
	blitter_tab
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
						tabs: [registers_tab, blitter_tab],
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

		layout.floatbox.children.push(... add.map((v, i) => ({ tabs: [v], x: 16*i, y:16*i, w: 300, h: 200 }) ));

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
