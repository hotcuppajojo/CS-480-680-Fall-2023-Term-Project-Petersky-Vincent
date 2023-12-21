// vertex_cube_shader.glsl

#version 460

layout (location = 0) in vec3 cubePosition;

out vec3 v_cubeTC;

layout (binding=0) uniform samplerCube samp;

uniform mat4 projection;
uniform mat4 modelView;

void main(void)
{
    v_cubeTC = cubePosition;
    vec4 v = vec4(cubePosition, 1.0);
    mat4 modelViewRot = mat4(mat3(modelView));
    gl_Position = (projection * modelViewRot) * v;
}