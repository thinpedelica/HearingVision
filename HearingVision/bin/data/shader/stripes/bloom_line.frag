#version 330

in vec4 gsColor;

out vec4 outputColor;

void main(){
    float a = pow(gsColor.a,2.0);
    outputColor = vec4(gsColor.rgb, a);
}
