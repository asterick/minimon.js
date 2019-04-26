#version 300 es

in vec2 vertex;
out vec2 position;

void main(void) {
    position = vertex;
    gl_Position = vec4(vertex * vec2(1.0, -1.0), 1.0, 1.0);
}
