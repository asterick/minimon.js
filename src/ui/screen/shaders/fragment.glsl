#version 300 es

precision mediump float;

uniform mediump usampler2D vram;
uniform float time;

const vec3 background = vec3(0.75, 0.86, 0.63);
const vec3 foreground = vec3(0.05);

in vec2 position;
out vec4 fragColor;

bool toggle(ivec2 pixel) {
	uint byte = texelFetch(vram, ivec2(pixel.x, pixel.y / 8), 0).r;
	return (byte & uint(1 << (pixel.y % 8)))  >= 1u;
}

void main(void) {
	vec2 pixel = vec2((position + 1.0) * vec2(96.0, 64.0) / 2.0);
	float distance = 1.0 - length(fract(pixel) - vec2(0.5));
	float intensity = toggle(ivec2(pixel)) ? distance : 0.0;
	float alpha = time * (1000.0 / 30.0);

	vec3 color = (foreground * intensity) + (background * (1.0 - intensity));

	fragColor = vec4(color, alpha);
}
