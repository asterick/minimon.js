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

#version 300 es

precision mediump float;

uniform mediump usampler2D vram;
uniform float time;
uniform int frame;

uniform vec3 color_light;
uniform vec3 color_dark;

uniform bool simulate_gray;
uniform bool dot_mask;
uniform float analog;

in vec2 position;
out vec4 fragColor;

vec2 fetch(vec2 pixel) {
	float a = float(texelFetch(vram, ivec2(pixel), 0).r) / 255.0;
	float b = float(texelFetch(vram, ivec2(pixel) + ivec2(0.0, 64.0), 0).r) / 255.0;

	if (frame > 0) {
		return vec2(a, b);
	} else {
		return vec2(b, a);
	}
}

float lcd(vec2 pixel) {
	return clamp(1.0 - length(fract(pixel) - vec2(0.5)) * 0.75, 0.0, 1.0);
}

void main(void) {
	vec2 pixel = vec2((position + 1.0) * vec2(96.0, 64.0) / 2.0);
	vec2 colors = fetch(pixel);

	float intensity;

	// simulated gray
	if (simulate_gray) {
		intensity = (colors.r + colors.g) / 2.0;
	} else {
		intensity = colors.r;
	}

	// Dot mask suport
	if (dot_mask) {
		intensity *= lcd(pixel);
	}

	fragColor.rgb = mix(color_dark, color_light, vec3(intensity));
	fragColor.a = time * (1000.0 / analog);
}
