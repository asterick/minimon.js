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

import { useRef } from "react";
import DockLayout, { type LayoutData, type LayoutBase, type TabData } from 'rc-dock'

import Screen from "./screen";
import Registers from "./registers";
import Memory from "./memory";
import Disassembler from "./disassemble";
import Settings from "./settings";
import Blitter from "./blitter";

import 'normalize.css/normalize.css';
import 'rc-dock/dist/rc-dock.css';
import "./style.less";

const screen_tab: TabData = {
	id: 'screen',
	minWidth: 96,
	minHeight: 64,
	title: 'Screen',
	content: <Screen />,
	closable: false
};
const settings_tab: TabData = {id: 'settings', title: 'Settings', content: <Settings />, closable: false };

const blitter_tab: TabData = {id: 'blitter', title: 'Blitter', content: <Blitter />, closable: true };
const registers_tab: TabData = {id: 'registers', title: 'Registers', content: <Registers />, closable: true };
const disassembly_tab: TabData = {id: 'disassembly', title: 'Disassembly', content: <Disassembler />, closable: true };
const memory_tab: TabData = {id: 'memory', title: 'Memory', content: <Memory baseAddress={0x1000} memory='ram' />, closable: false };
const eeprom_tab: TabData = {id: 'eeprom', title: 'EEPROM', content: <Memory baseAddress={0} memory='eeprom' />, closable: true };

const all_tabs = [
	screen_tab,
	registers_tab,
	disassembly_tab,
	memory_tab,
	eeprom_tab,
	settings_tab,
	blitter_tab
];

const defaultLayout: LayoutData = {
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

interface SavedNode {
	tabs?: { id?: string }[];
	children?: SavedNode[];
}

function findTabs(node: SavedNode): string[] {
	if (node.tabs) {
		return node.tabs.map((t) => t.id).filter((id): id is string => id !== undefined);
	} else if (node.children) {
		return node.children.flatMap(findTabs);
	}

	return [];
}

function preserveLayout(layout: LayoutBase) {
	window.localStorage.setItem("minimon-layout", JSON.stringify(layout));
}

export default function UI() {
	const loaded = useRef(false);

	function reloadLayout(r: DockLayout | null) {
		if (loaded.current || !r) return;

		loaded.current = true;

		const preserved = window.localStorage.getItem("minimon-layout");

		if (!preserved) return;

		const layout = JSON.parse(preserved);

		/* This will shove tabs not found in arbitrary places */
		const found_tabs = [layout.dockbox, layout.floatbox, layout.windowbox, layout.maxbox]
			.filter(Boolean)
			.flatMap(findTabs);
		const add = all_tabs.filter((t) => (!t.closable && found_tabs.indexOf(t.id!) < 0));

		layout.floatbox.children.push(... add.map((v, i) => ({ tabs: [v], x: 16*i, y: 16*i, w: 300, h: 200 }) ));

		/* Give the layout to our manager */
		r.loadLayout(layout);
	}

	return <DockLayout
		ref={reloadLayout}
		defaultLayout={defaultLayout}
		onLayoutChange={preserveLayout}
		style={{position: 'absolute', left: 0, top: 0, right: 0, bottom: 0}}
		/>;
}
