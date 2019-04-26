#version 300 es

precision mediump float;

uniform mediump usampler2D vram;
uniform float time;
uniform int frame;

uniform vec3 background;
uniform vec3 foreground;

in vec2 position;
out vec4 fragColor;

bool toggle(ivec2 pixel) {
	uint byte = texelFetch(vram, ivec2(pixel.x, pixel.y / 8), 0).r;

	return (byte & uint(1 << (pixel.y & 7)))  >= 1u;
}

bool current_pixel(vec2 pixel) {
	if (frame > 0) {
		return toggle(ivec2(pixel));
	} else {
		return toggle(ivec2(pixel) + ivec2(0, 64));
	}
}

bool previous_pixel(vec2 pixel) {
	if (frame > 0) {
		return toggle(ivec2(pixel) + ivec2(0, 64));
	} else {
		return toggle(ivec2(pixel));
	}
}

float lcd(vec2 pixel) {
	float distance = clamp(1.0 - length(fract(pixel) - vec2(0.5)) * 0.75, 0.0, 1.0);
	return current_pixel(pixel) ? distance : 0.0;
}

float simulated_gray(vec2 pixel) {
	float intensity1 = current_pixel(pixel) ? 1.0 : 0.0;
	float intensity2 = previous_pixel(pixel) ? 1.0 : 0.0;

	return (intensity1 + intensity2) / 2.0;
}

float basic(vec2 pixel) {
	return current_pixel(pixel) ? 1.0 : 0.0;
}

void main(void) {
	vec2 pixel = vec2((position + 1.0) * vec2(96.0, 64.0) / 2.0);
	float blur = time * (1000.0 / 30.0);

	float intensity = lcd(pixel);

	// Setting alpha to 100% will disable analog blending
	fragColor = vec4(mix(background, foreground, vec3(intensity)), blur);
}
