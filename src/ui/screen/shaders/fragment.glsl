#version 300 es

precision mediump float;

uniform mediump usampler2D vram;
uniform float time;
uniform float clock;
uniform int frame;

uniform bool simulate_gray;
uniform float analog;

in vec2 position;
out vec4 fragColor;

vec3 fetch(ivec2 pixel) {
	if (!simulate_gray) {
		return vec3(texelFetch(vram, ivec2(pixel) + ivec2(0, frame * 64), 0)) / 255.0;
	} else {
		vec3 o = vec3(0.0);
		for (int i = 0; i < 4; i++) {
			o += vec3(texelFetch(vram, ivec2(pixel) + ivec2(0, i * 64), 0)) / 255.0;
		}
		return o / 4.0;
	}
}

void main(void) {
	vec2 pixel = vec2((position + 1.0) * vec2(96.0, 64.0) / 2.0);

	fragColor.rgb = fetch(ivec2(pixel));
	fragColor.a = time * (1000.0 / analog);
}
