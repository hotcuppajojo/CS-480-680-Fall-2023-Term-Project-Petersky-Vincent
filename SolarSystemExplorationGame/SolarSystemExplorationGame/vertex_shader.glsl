#version 460

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_tc; 
uniform bool hasTexture;

smooth out vec3 color;
out vec2 tc;

layout (binding=0) uniform sampler2D sp;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform bool hasTC;

void main(void)
{
    vec4 v = vec4(v_position, 1.0);
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
    color = v_color;
    tc = v_tc;
}