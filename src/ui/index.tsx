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

import {
	DockviewReact,
	DockviewDefaultTab,
	themeLight,
	type DockviewApi,
	type DockviewReadyEvent,
	type IDockviewPanelProps,
	type IDockviewPanelHeaderProps,
} from 'dockview';

import Screen from "./screen";
import Registers from "./registers";
import Memory from "./memory";
import Disassembler from "./disassemble";
import Settings from "./settings";
import Blitter from "./blitter";

import 'normalize.css/normalize.css';
import 'dockview/dist/styles/dockview.css';
import "./style.less";

const LAYOUT_KEY = "minimon-dockview";

interface MemoryParams {
	baseAddress: number;
	memory: 'ram' | 'eeprom';
}

function MemoryPanel(props: IDockviewPanelProps<MemoryParams>) {
	return <Memory baseAddress={props.params.baseAddress} memory={props.params.memory} />;
}

const components = {
	screen: Screen,
	settings: Settings,
	registers: Registers,
	blitter: Blitter,
	disassembly: () => <Disassembler />,
	memory: MemoryPanel,
};

const tabComponents = {
	permanent: (props: IDockviewPanelHeaderProps) => <DockviewDefaultTab hideClose {...props} />,
};

// Panels the user can't close; restored as floating windows if a saved
// layout is missing them
const REQUIRED_PANELS = [
	{ id: 'screen', component: 'screen', title: 'Screen' },
	{ id: 'settings', component: 'settings', title: 'Settings' },
	{ id: 'memory', component: 'memory', title: 'Memory', params: { baseAddress: 0x1000, memory: 'ram' } },
];

function defaultLayout(api: DockviewApi) {
	api.addPanel({ id: 'screen', component: 'screen', title: 'Screen', tabComponent: 'permanent' });
	api.addPanel({
		id: 'disassembly', component: 'disassembly', title: 'Disassembly',
		position: { referencePanel: 'screen', direction: 'right' }
	});
	api.addPanel({
		id: 'memory', component: 'memory', title: 'Memory', tabComponent: 'permanent',
		params: { baseAddress: 0x1000, memory: 'ram' },
		position: { referencePanel: 'disassembly' }
	});
	api.addPanel({
		id: 'eeprom', component: 'memory', title: 'EEPROM',
		params: { baseAddress: 0, memory: 'eeprom' },
		position: { referencePanel: 'disassembly' }
	});
	api.addPanel({
		id: 'settings', component: 'settings', title: 'Settings', tabComponent: 'permanent',
		position: { referencePanel: 'screen', direction: 'below' }
	});
	api.addPanel({
		id: 'registers', component: 'registers', title: 'Registers',
		position: { referencePanel: 'settings', direction: 'below' }
	});
	api.addPanel({
		id: 'blitter', component: 'blitter', title: 'Blitter',
		position: { referencePanel: 'registers' }
	});

	api.getPanel('registers')?.api.setActive();
	api.getPanel('disassembly')?.api.setActive();
}

function onReady(event: DockviewReadyEvent) {
	const api = event.api;

	// The rc-dock layout format is incompatible; drop any stale copy
	window.localStorage.removeItem("minimon-layout");

	const preserved = window.localStorage.getItem(LAYOUT_KEY);
	let restored = false;

	if (preserved) {
		try {
			api.fromJSON(JSON.parse(preserved));
			restored = true;
		} catch (e) {
			console.warn("Could not restore saved layout", e);
			api.clear();
		}
	}

	if (!restored) {
		defaultLayout(api);
	} else {
		// Re-add any non-closable panels missing from the saved layout
		REQUIRED_PANELS
			.filter((panel) => !api.getPanel(panel.id))
			.forEach((panel, i) => {
				api.addPanel({
					...panel,
					tabComponent: 'permanent',
					floating: { x: 16 * (i + 1), y: 16 * (i + 1), width: 300, height: 200 }
				});
			});
	}

	api.onDidLayoutChange(() => {
		window.localStorage.setItem(LAYOUT_KEY, JSON.stringify(api.toJSON()));
	});
}

export default function UI() {
	return <div style={{position: 'absolute', left: 0, top: 0, right: 0, bottom: 0}}>
		<DockviewReact
			components={components}
			tabComponents={tabComponents}
			theme={themeLight}
			onReady={onReady}
			/>
	</div>;
}
