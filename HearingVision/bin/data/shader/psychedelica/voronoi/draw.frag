#version 150

// original http://glslsandbox.com/e#53836.0
// Colorful Voronoi
// By: Brandon Fogerty
// bfogerty at gmail dot com
// xdpixel.com

uniform float time;
uniform float level;
uniform float color;
uniform float high;
uniform float mid;
uniform float low;
uniform vec2 resolution;

out vec4 outputColor;

vec2 hash(vec2 p) {
    mat2 m = mat2(  13.85, 47.77,
                    99.41, 88.48
                );

    return fract(sin(m * p) * 46738.29);
}

float voronoi(vec2 p) {
    vec2 g = floor(p);
    vec2 f = fract(p);

    float distanceToClosestFeaturePoint = 10.0;
    for(int y = -1; y <= 1; y++) {
        for(int x = -1; x <= 1; x++) {
            vec2 latticePoint = vec2(x, y);
            float currentDistance = distance(latticePoint + hash(g + latticePoint), f);
            distanceToClosestFeaturePoint = min(distanceToClosestFeaturePoint, currentDistance);
        }
    }

    return distanceToClosestFeaturePoint;
}

void main( void ) {
    vec2 uv = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;

    float trans_level = ceil(10.f * level);
    float color_level = ceil(10.f * color);

    float offset = voronoi(uv * 10.0 + vec2(time)) * 2.0 * mid;
    float t = 1.0 / abs(((uv.x + sin(uv.y + time * trans_level)) + offset) * 30.0);

    float r = voronoi( uv * 1.0 ) * color_level;
    vec3 finalColor = vec3(10.0 * uv.y, 2.0, 1.0 * r) * t;

    outputColor = vec4(finalColor, 1.0);
}