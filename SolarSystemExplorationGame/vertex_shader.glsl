// vertex_shader.glsl

#version 460

struct Lights {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position; // World space position
    vec3 positionViewSpace; // View space position
};

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_tc; 
layout (location = 3) in vec3 v_normal;

layout (binding=0) uniform sampler2D sp;
layout (binding=1) uniform sampler2D sp1;

uniform bool hasTexture;
uniform bool hasNormalMap;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normMatrix;

uniform Lights light;
uniform Material material;

out vec3 colorOut;
out vec2 tcOut;
out vec3 normalViewSpace;
out vec3 lightDirViewSpace;
out vec3 fragPosViewSpace;

void main(void) {
    vec4 worldSpacePosition = modelMatrix * vec4(v_position, 1.0);
    vec4 viewSpacePosition = viewMatrix * worldSpacePosition;

    gl_Position = projectionMatrix * viewSpacePosition;
    colorOut = v_color;
    tcOut = v_tc;

    normalViewSpace = normMatrix * v_normal;
    lightDirViewSpace = light.positionViewSpace - viewSpacePosition.xyz;
    fragPosViewSpace = viewSpacePosition.xyz;
}