#version 150

uniform float time;
uniform float level;
uniform float high;
uniform float mid;
uniform float low;
uniform vec2 resolution;

out vec4 outputColor;

const int kLoopMax = 10;
const float kRBase = 1.3;
const float kGBase = 0.999;
const float kBBase = 0.7;

void main() {
    vec3 p = vec3(gl_FragCoord.xy / resolution.y, sin(time * 0.2) * 0.4);
    float r = kRBase + high;
    float b = kBBase + mid;
    float g = kGBase + low;
    for (int i = 0; i < kLoopMax; i++) {
        vec3 result = vec3(r, g, b) * abs((abs(p) / dot(p, p)) - vec3(0.7, 0.4, cos(time * 0.2) * 0.4));
        p = step(level, float(i+.1)) * p
             + step(float(i+.1), level) * result.xzy;
    }
    outputColor = vec4(p, 1.0);
}
