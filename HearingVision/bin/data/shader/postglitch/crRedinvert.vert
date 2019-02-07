#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
out vec3 pos;

void main(void)
{
    gl_Position = modelViewProjectionMatrix * position;
    pos = position.xyz;
}