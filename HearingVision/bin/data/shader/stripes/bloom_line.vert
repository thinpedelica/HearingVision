#version 330

in vec4 color;
in vec3 position;

out vec4 vColor;
out vec3 vPosition;

void main() {
    vColor = color;
    vPosition = position;
}
