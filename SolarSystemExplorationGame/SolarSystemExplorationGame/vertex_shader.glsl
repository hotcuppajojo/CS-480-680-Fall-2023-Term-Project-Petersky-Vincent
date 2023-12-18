#version 460

struct PositionalLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 spec;
    vec3 position;
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

layout (binding=0) uniform sampler2D sp;
layout (binding=1) uniform sampler2D sp1;

uniform bool hasTexture;
uniform bool hasNormalMap;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normMatrix;

uniform vec4 GlobalAmbient;
uniform PositionalLight light;
uniform Material material;

smooth out vec3 color;
out vec3 varNorm;
out vec3 varLdir;
out vec3 varPos;
out vec2 tc;

void main(void)
{
    vec4 v = vec4(v_position, 1.0);
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
    color = v_color;
    tc = v_tc;

    varPos = (viewMatrix * modelMatrix * vec4(v_position, 1.0f)).xyz;
    varLdir = light.position - varPos;
    varNorm = normMatrix * v_color;
}