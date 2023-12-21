#version 460

in vec3 v_cubeTC;

layout (binding=0) uniform samplerCube samp;

uniform mat4 projection;
uniform mat4 modelView;

out vec4 fragColor;

void main(void)
{
    fragColor = texture(samp, v_cubeTC);
}