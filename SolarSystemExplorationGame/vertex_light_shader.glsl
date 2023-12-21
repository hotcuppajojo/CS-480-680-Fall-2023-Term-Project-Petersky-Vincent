// vertex_light_shader.glsl

#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tc; 
layout (location = 3) in vec3 v_normal;

uniform mat4 lightProjection;
uniform mat4 lightView;
uniform mat4 lightModel;
uniform mat3 normMatrix; // Normal matrix for transforming normals to view space

uniform bool lightHasTexture;
uniform bool lightHasTC;

out vec3 colorOut;
out vec2 tcOut;
out vec3 normalViewSpace;
out vec3 fragPosViewSpace;

void main(void) {
    vec4 viewSpacePosition = lightView * lightModel * vec4(position, 1.0);
    gl_Position = lightProjection * viewSpacePosition;
    colorOut = color;
    tcOut = tc;

    normalViewSpace = normMatrix * v_normal;
    fragPosViewSpace = viewSpacePosition.xyz;
}