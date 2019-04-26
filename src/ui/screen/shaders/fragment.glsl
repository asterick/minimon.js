#version 300 es

precision mediump float;

uniform float bb;
uniform uint memory[768];
uniform uint previous[768];
uniform float time;

in vec2 position;
out vec4 fragColor;

void main(void) {
	uvec2 pixel = uvec2((position + 1.0) * vec2(96.0, 64.0) / 2.0);
	uint address = (pixel.y / 8u) * 96u + pixel.y;
	uint toggle = uint(bb) & (1u << (pixel.y % 8u));
	vec3 color = (toggle >= 1u) ? vec3(0.0) : vec3(255.0);

	fragColor = vec4(color, time * (1000.0f / 60.0f));
}