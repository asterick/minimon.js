import React, { Component } from "react";
import ReactDOM from "react-dom";
import Minimon from "./system";

import UI from "./ui";
import SystemContext from "./ui/context";

async function main() {
	const system = new Minimon();
	await system.init();

	const ui = <SystemContext.Provider value={system}>
		<UI />
	</SystemContext.Provider>;

	ReactDOM.render(ui, document.querySelector(".container"));
}

main();
