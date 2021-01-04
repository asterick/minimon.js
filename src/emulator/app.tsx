/*
 * MIT License
 *
 * Copyright (c) 2019-2021, Bryon Vandiver
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

import { ipcRenderer } from "electron";

import * as system from "./system";
import * as screen from "./screen";
import './index.less';

ipcRenderer.on('load-file', (event, binary) => {
	system.load(binary);
});

console.log(ipcRenderer.sendSync('load-data', 'never'))
console.log(ipcRenderer.sendSync('save-data', '\x00', "asdf"))

console.log(ipcRenderer.sendSync('save-data', 'default-eeprom', "asdf"))
console.log(ipcRenderer.sendSync('load-data', 'default-eeprom', 'utf-8'))
console.log(ipcRenderer.sendSync('load-data', 'default-eeprom'))

async function main() {
	screen.init();
	await system.init();

	system.start();
}

main();
