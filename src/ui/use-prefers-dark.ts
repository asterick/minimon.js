import { useSyncExternalStore } from "react";

const query = window.matchMedia("(prefers-color-scheme: dark)");

function subscribe(listener: () => void): () => void {
	query.addEventListener("change", listener);
	return () => query.removeEventListener("change", listener);
}

// Tracks the system color-scheme preference; the stylesheet tokens
// follow it automatically, this hook is for JS consumers (dockview's
// theme object)
export function usePrefersDark(): boolean {
	return useSyncExternalStore(subscribe, () => query.matches);
}
