#version 460

layout (location = 0) in vec3 lightposition;
layout (location = 1) in vec3 lightcolor;
layout (location = 2) in vec2 lightTC; 

layout (binding=0) uniform sampler2D sampler;

uniform bool lightHasTexture;
uniform bool lightHasTC;

uniform mat4 lightProjection;
uniform mat4 lightView;
uniform mat4 lightModel;

smooth out vec3 color;
out vec2 tc;

void main(void)
{
    vec4 v = vec4(lightposition, 1.0);
    gl_Position = (lightProjection * lightView * lightModel) * v;
    color = lightcolor;
    tc = lightTC;
}