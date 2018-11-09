#version 330

uniform mat4 modelViewProjectionMatrix;
uniform sampler2DRect u_pos_tex;

in vec2  texcoord;

out vec2 v_texcoord;

void main() {
    v_texcoord = texcoord;

    gl_Position = modelViewProjectionMatrix * vec4(texture(u_pos_tex, v_texcoord).xy, 1.0, 1.0);
    gl_PointSize = 0.1;
}
