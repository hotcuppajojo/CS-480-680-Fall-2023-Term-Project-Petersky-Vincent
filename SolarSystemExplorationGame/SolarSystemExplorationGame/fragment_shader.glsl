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

layout (binding=0) uniform sampler2D sp;
layout (binding=1) uniform sampler2D sp1;

uniform bool hasTexture;
uniform bool hasNormalMap;

uniform vec4 GlobalAmbient;
uniform PositionalLight light;
uniform Material material;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normMatrix;
uniform bool hasTC;

smooth in vec3 color;
in vec3 varNorm;
in vec3 varLdir;
in vec3 varPos;
in vec2 tc;
out vec4 frag_color;

void main(void)
{
    vec3 L = normalize(varLdir);
    vec3 N;
    if(hasNormalMap)
        N = normalize(varNorm + texture(sp1, tc).xyz*2-1);
    else
        N = normalize(varNorm);
    vec3 V = normalize(-varPos);
    vec3 R = normalize(reflect(-L, N));
    vec4 initialColor;

    if(hasTexture)
        initialColor = texture(sp,tc);
    else 
        initialColor = vec4(color.rgb, 1.0);

    float cosTheta = dot(L, N);
    float cosPhi = dot(R, V);
    vec3 amb = ((GlobalAmbient)+(initialColor*light.ambient*material.ambient)/1).xyz;
    vec3 dif = light.diffuse.xyz * material.diffuse.xyz * initialColor.xyz * max(0.0, cosTheta);
    vec3 spc = light.spec.xyz*pow(max(0.0, cosPhi), material.shininess);
    
    frag_color = vec4(amb+dif+spc, 1);
}