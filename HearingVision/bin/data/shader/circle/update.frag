#version 330

uniform sampler2DRect u_pos_tex;
uniform vec2 u_target_pos;
uniform bool u_is_move;
uniform float u_velocity;

in vec2 v_texcoord;

layout(location = 0) out vec4 posOut;

const float kSpeed = 1.0;

void main() {
    vec2 pos = vec2(texture(u_pos_tex, v_texcoord.st).xy);
    vec2 vect = vec2(texture(u_pos_tex, v_texcoord.st).zw);

    vec2 move_dist = normalize(u_target_pos.xy - pos.xy) * 0.2;
    vec2 new_vect = normalize(vect + move_dist);

    vec4 result = vec4(pos +  (kSpeed * u_velocity) * new_vect, new_vect);
    if (!u_is_move) {
      result.zw = vect;
    }

    posOut = result;
}
