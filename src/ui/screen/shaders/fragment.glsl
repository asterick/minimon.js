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

float fetch(ivec2 pixel) {
	if (!simulate_gray) {
		return float(texelFetch(vram, ivec2(pixel) + ivec2(0, frame * 64), 0).r) / 255.0;
	} else {
		float o = 0.0;
		for (int i = 0; i < 4; i++) {
			o += float(texelFetch(vram, ivec2(pixel) + ivec2(0, i * 64), 0).r) / 255.0;
		}
		return o / 4.0;
	}
}

float lcd(vec2 pixel) {
	return clamp(1.0 - length(vec2(0.5) - fract(1.0 - pixel)) * 0.75, 0.0, 1.0);
}

void main(void) {
	vec2 pixel = vec2((position + 1.0) * vec2(96.0, 64.0) / 2.0);
	float intensity = fetch(ivec2(pixel));

	// Dot mask suport
	if (dot_mask) {
		intensity *= lcd(pixel);
	}

	fragColor.rgb = mix(color_dark, color_light, vec3(intensity));
	fragColor.a = time * (1000.0 / analog);
}
