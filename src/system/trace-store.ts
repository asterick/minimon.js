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

/*
 * Persistence for the debugger's classification maps. The 2MB cartridge
 * map exceeds what localStorage can hold, so both maps live in
 * IndexedDB: the BIOS map under a fixed key (the BIOS never changes),
 * cartridge maps keyed by a hash of the ROM contents.
 */

const DB_NAME = "minimon-trace";
const STORE = "maps";

export const BIOS_KEY = "bios";

let database: Promise<IDBDatabase> | null = null;

function open(): Promise<IDBDatabase> {
	if (!database) {
		database = new Promise((resolve, reject) => {
			const request = indexedDB.open(DB_NAME, 1);

			request.onupgradeneeded = () => {
				request.result.createObjectStore(STORE);
			};
			request.onsuccess = () => resolve(request.result);
			request.onerror = () => reject(request.error);
		});
	}

	return database;
}

export async function loadMap(key: string): Promise<Uint8Array | null> {
	const db = await open();

	return new Promise((resolve, reject) => {
		const request = db.transaction(STORE, "readonly").objectStore(STORE).get(key);

		request.onsuccess = () => resolve(request.result instanceof Uint8Array ? request.result : null);
		request.onerror = () => reject(request.error);
	});
}

export async function saveMap(key: string, data: Uint8Array): Promise<void> {
	const db = await open();

	return new Promise((resolve, reject) => {
		// Copy out of WASM memory: the live view's buffer can't be
		// structured-cloned while it keeps mutating
		const tx = db.transaction(STORE, "readwrite");
		tx.objectStore(STORE).put(data.slice(), key);

		tx.oncomplete = () => resolve();
		tx.onerror = () => reject(tx.error);
	});
}

// FNV-1a over the ROM contents; identifies a cartridge for map storage
export function cartridgeKey(rom: Uint8Array): string {
	let hi = 0x811c9dc5 | 0;
	let lo = 0x01000193 | 0;

	for (let i = 0; i < rom.length; i++) {
		hi = ((hi ^ rom[i]) * 0x01000193) >>> 0;
		lo = ((lo ^ rom[i] ^ (hi >>> 24)) * 0x01000193) >>> 0;
	}

	return `cart:${hi.toString(16).padStart(8, "0")}${lo.toString(16).padStart(8, "0")}`;
}
