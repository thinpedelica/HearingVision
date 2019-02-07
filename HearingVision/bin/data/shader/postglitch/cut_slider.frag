#version 150

uniform sampler2DRect image;
uniform int imgWidth,imgHeight;
uniform float rand;
uniform int flags;
uniform vec2 blur_vec;
int flgs;
float pix_w,pix_h;

in vec3 pos;
out vec4 outputColor;

void main (void)
{
    pix_w = 1.0;
    pix_h = 1.0;
    flgs = flags;
    
    vec2 texCoord = vec2(pos.x, pos.y);
    vec4 col = texture2DRect(image,texCoord);
    vec4 col_s = texture2DRect(image,texCoord + vec2(floor(sin(pos.y/30.0*rand+rand*rand))*30.0*rand,0));
    
    col = col_s;
    
	outputColor.rgba = col.rgba;
}
