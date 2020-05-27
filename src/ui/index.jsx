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
import SystemContext from "./context";

import Screen from "./screen";
import Registers from "./registers";
import Memory from "./memory";
import Disassembler from "./disassemble";

import style from "./style.less";

let screen_tab = {id: 'screen', minWidth: 96, minHeight:64, title: 'Screen', content: <Screen />, closable: false };
let registers_tab = {id: 'registers', title: 'Registers', content: <Registers /> };
let disassembly_tab = {id: 'disassembly', title: 'Disassembly', content: <Disassembler /> };
let memory_tab = {id: 'memory', title: 'Memory', content: <Memory baseAddress={0x1000} memory='ram' />};
let eeprom_tab = {id: 'eeprom', title: 'EEPROM', content: <Memory baseAddress={0} memory='eeprom' />};


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
							{ tabs: [disassembly_tab] },
							{ tabs: [memory_tab, eeprom_tab] }
						]
					}
				]
				
			}
		]
	}
};

export default class UI extends React.Component {
	static contextType = SystemContext;
	
	preserveLayout(layout) {
		window.localStorage.setItem("minimon-layout",JSON.stringify(layout));
	}

	reloadLayout(r) {
		let layout = window.localStorage.getItem("minimon-layout");
		
		if (layout) {
			try {
				r.loadLayout(JSON.parse(layout));
			} catch(e) {
				console.error("Could not load layout");
			}
		}
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
