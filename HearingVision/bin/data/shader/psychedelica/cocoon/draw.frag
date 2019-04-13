#version 150

// original http://glslsandbox.com/e#53210.0
uniform float time;
uniform float level;
uniform float color;
uniform float high;
uniform float mid;
uniform float low;
uniform vec2 resolution;

out vec4 outputColor;

const float Pi= 3.14159265;

float rand(int seed, float ray) {
    return mod(sin(float(seed) * 363.5346 + ray * 674.2454) * 6743.4365, 1.0);
}

void main( void ) {
    vec2 position = ( gl_FragCoord.xy / resolution.xy );
    position -= vec2(0.5);
    position *= 0.9 + sin(time) * 0.5 * level;

    position.y *= resolution.y / resolution.x;

    float ang = atan(position.y, position.x);
    float dist = length(position);
    outputColor.rgb = vec3(0.8, 0.8, color) * (pow(dist, -1.0) * 0.05);
    for (float ray = 0.0; ray < 60.0; ray += 0.5) {
        float rayang = rand(5234, ray) * 6.2 + (time * 0.04) * 10.0 * (rand(2546, ray) - rand(5785, ray)) - (rand(3545, ray) - rand(5467, ray));
        rayang = mod(rayang + sin(dist * ray * 4.8), Pi * 2.0);
        if (rayang < ang - Pi) {rayang += Pi * 2.0;}
        if (rayang > ang + Pi) {rayang -= Pi * 2.0;}
        float brite = .1 - abs(ang - rayang);
        brite -= dist * abs(0.3 - mid);
        if (brite > 0.0) {
            vec3 ccol = vec3(0.2 + 0.4 * rand(8644, ray), 0.4 + 0.4 * rand(4567, ray), 0.5 + 0.4 * rand(7354, ray)) * brite;
            outputColor.rgb += ccol * 0.7;
        }
    }
    outputColor.a = 1.0;
}