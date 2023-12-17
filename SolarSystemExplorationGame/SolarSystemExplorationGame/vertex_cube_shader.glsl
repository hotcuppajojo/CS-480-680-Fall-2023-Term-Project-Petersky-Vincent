#version 460

layout (location = 0) in vec3 position;

out vec3 cubeTC;

layout (binding=0) uniform samplerCube samp;

uniform mat4 projection;
uniform mat4 modelView;

void main(void)
{
    cubeTC = position;
    vec4 v = vec4(position, 1.0);
    gl_Position = (projection * modelView) * v;
}