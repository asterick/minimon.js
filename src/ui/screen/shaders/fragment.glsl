#version 300 es

precision mediump float;

uniform mediump usampler2D vram;
uniform float time;
uniform int frame;

uniform vec3 background;
uniform vec3 foreground;

uniform bool simulate_gray;
uniform bool dot_mask;
uniform float analog;

in vec2 position;
out vec4 fragColor;

bool toggle(ivec2 pixel) {
	uint byte = texelFetch(vram, ivec2(pixel.x, pixel.y / 8), 0).r;

	return (byte & uint(1 << (pixel.y & 7)))  >= 1u;
}

vec2 fetch(vec2 pixel) {
	float a = toggle(ivec2(pixel)) ? 1.0 : 0.0;
	float b = toggle(ivec2(pixel) + ivec2(0, 64)) ? 1.0 : 0.0;

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

	fragColor.rgb = mix(background, foreground, vec3(intensity));
	fragColor.a = time * (1000.0 / analog);
}
