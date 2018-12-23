#version 330

uniform float time;
uniform float scale;
uniform float waves;
uniform float speed;
uniform vec3  color;
uniform vec2  resolution;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

const float level = 0.3;

float band(vec2 pos, float amp, float freq) {
  float wave = 0.5 * amp * sin(2.0 * PI * freq * pos.x + time) / 2.0;
  float light = clamp(amp * freq * 0.002, 0.001 + 0.001 / scale, 5.0) * scale / abs(wave - pos.y);
  return light;
}

void main( void ) {
  vec2 pos = (gl_FragCoord.xy / resolution.xy);
  pos.x += 1.0;
  pos.y -= 0.5;

  const float line_num = 64.;
  float step = 1.0 / line_num;
  float ts = time * speed;
  float spectrum = level;
  for (float i = 0.; i < line_num; i++) {
    float amp  = scale * sin(ts + pos.x * step * i);
    float freq = waves * sin(ts + pos.x * step * i * 0.3);
    spectrum += band(pos, amp, freq) / pow(line_num, 0.25);
  }

  outputColor = vec4(color * spectrum, spectrum);
}