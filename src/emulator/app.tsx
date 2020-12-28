import * as system from "./system";

import './index.less';

async function main() {
	await system.init(true);
	system.start();
}

main();
