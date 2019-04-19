import React, { Component } from "react";
import ReactDOM from "react-dom";

import UI from "./ui";
import Minimon from "./system";

const system = new Minimon();

async function main() {
	await system.init();

	ReactDOM.render(<UI system={system}/>, document.querySelector(".container"));
}

main();
