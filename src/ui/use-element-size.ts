import { useLayoutEffect, useState } from "react";

export interface ElementSize {
	width: number;
	height: number;
}

// Tracks an element's client size with a ResizeObserver (replaces
// react-virtualized's AutoSizer)
export function useElementSize(ref: React.RefObject<HTMLElement | null>): ElementSize {
	const [size, setSize] = useState<ElementSize>({ width: 0, height: 0 });

	useLayoutEffect(() => {
		const element = ref.current;
		if (!element) return;

		const observer = new ResizeObserver(() => {
			setSize((previous) => {
				const width = element.clientWidth;
				const height = element.clientHeight;

				return (previous.width == width && previous.height == height)
					? previous : { width, height };
			});
		});

		observer.observe(element);
		return () => observer.disconnect();
	}, [ref]);

	return size;
}
