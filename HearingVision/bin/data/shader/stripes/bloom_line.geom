#version 330

layout (lines) in;
layout (triangle_strip, max_vertices = 12) out;

uniform mat4 modelViewProjectionMatrix;

uniform vec3 camPosition;

in vec4 vColor[];
in vec3 vPosition[];

out vec4 gsColor;

void main(){
    vec3 pos_0 = vec3(vPosition[0].xy, 0.0f);
    vec3 pos_1 = vec3(vPosition[1].xy, 0.0f);
    float width = vPosition[0].z;

    vec3 norm = normalize(camPosition - pos_0);
    vec3 lineDirection = normalize(pos_1 - pos_0);
    vec3 lineWidthDirection = normalize(cross(norm, lineDirection));

    vec4 color_0 = vColor[0];
    color_0.a = 0.0;

    vec4 color_1 = vColor[1];
    color_1.a = 0.0;

    //4triangle out
    gl_Position = modelViewProjectionMatrix * vec4(pos_0, 1.0);
    gsColor = vColor[0];
    EmitVertex();

    gl_Position = modelViewProjectionMatrix * vec4(pos_0 + lineWidthDirection * width, 1.0);
    gsColor = color_0;
    EmitVertex();

    gl_Position = modelViewProjectionMatrix * vec4(pos_1, 1.0);
    gsColor = vColor[1];
    EmitVertex();
    EndPrimitive();


    gl_Position = modelViewProjectionMatrix * vec4(pos_0 + lineWidthDirection * width, 1.0);
    gsColor = color_0;
    EmitVertex();

    gl_Position = modelViewProjectionMatrix * vec4(pos_1 + lineWidthDirection * width, 1.0);
    gsColor = color_1;
    EmitVertex();

    gl_Position = modelViewProjectionMatrix * vec4(pos_1, 1.0);
    gsColor = vColor[1];
    EmitVertex();
    EndPrimitive();


    gl_Position = modelViewProjectionMatrix * vec4(pos_0, 1.0);
    gsColor = vColor[0];
    EmitVertex();

    gl_Position = modelViewProjectionMatrix * vec4(pos_0 - lineWidthDirection * width, 1.0);
    gsColor = color_0;
    EmitVertex();

    gl_Position = modelViewProjectionMatrix * vec4(pos_1, 1.0);
    gsColor = vColor[1];
    EmitVertex();
    EndPrimitive();


    gl_Position = modelViewProjectionMatrix * vec4(pos_0 - lineWidthDirection * width, 1.0);
    gsColor = color_0;
    EmitVertex();

    gl_Position = modelViewProjectionMatrix * vec4(pos_1 - lineWidthDirection * width, 1.0);
    gsColor = color_1;
    EmitVertex();

    gl_Position = modelViewProjectionMatrix * vec4(pos_1, 1.0);
    gsColor = vColor[1];
    EmitVertex();
    EndPrimitive();
}
