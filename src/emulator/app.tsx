import Minimon from "./system";
import Screen from "./screen";

import './index.less';

async function main() {
	const system = new Minimon("default");
	const screen = new Screen(system);

	await system.init(true);
	system.running = true;

	document.body.appendChild(screen.element);
}

main();
