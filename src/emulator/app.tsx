import Minimon from "./system";

import './index.less';

async function main() {
	const system = new Minimon("default");

	await system.init(true);
	system.running = true;

	document.body.appendChild(system.element);
}

main();
