#version 150

// original http://glslsandbox.com/e#53560.0

uniform float time;
uniform float level;
uniform float high;
uniform float mid;
uniform float low;
uniform vec2 resolution;

out vec4 outputColor;

float Pi= 3.14159265;

vec3 hsv2rgb(vec3 c) {
    c = vec3(c.x, clamp(c.yz, 0.0, 1.0));
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(cos(c.xxx + K.xyz) * Pi - (K.www));
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(void) {
    vec4 in_val = vec4(vec2(mid, low) * resolution, resolution);
    vec2 p = gl_FragCoord.xy * 1.;
    vec2 q = ((p) + (p) - (in_val.ba)) / (in_val.b);

    for(int i = 0; i < 10; i++) {
        q = atan(abs(abs(atan(q * Pi)) / dot(atan(q * Pi), (q * Pi)) -  in_val.xy / in_val.zw));	
        if (length(q) > .1 && length(q) < .125) break;
    }

    outputColor = vec4(hsv2rgb((vec3(q, q.x / q.y).xyz) + vec3(sin(time * 8.) / 50., .9, .5)), 1.0);
}