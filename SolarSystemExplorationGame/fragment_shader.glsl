// fragment_shader.glsl

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

layout (binding=0) uniform sampler2D sp;
layout (binding=1) uniform sampler2D sp1;

uniform bool hasTexture;
uniform bool hasNormalMap;

uniform Lights light;
uniform Material material;
uniform vec4 GlobalAmbient;

in vec3 colorOut;
in vec2 tcOut;
in vec3 normalViewSpace;
in vec3 lightDirViewSpace;
in vec3 fragPosViewSpace;
out vec4 frag_color;

void main(void) {
    vec3 L = normalize(lightDirViewSpace);
    vec3 N;

    if (hasNormalMap) {
        vec3 texNormal = texture(sp1, tcOut).xyz;
        N = normalize(normalViewSpace + (texNormal * 2.0 - 1.0)); // Adjusted normal
    } else {
        N = normalize(normalViewSpace); // Use interpolated normal
    }

    vec3 V = normalize(-fragPosViewSpace);
    vec3 R = normalize(reflect(-L, N));

    vec4 initialColor = hasTexture ? texture(sp, tcOut) : vec4(colorOut, 1.0);

    vec3 ambient = (GlobalAmbient.xyz + (light.ambient * material.ambient).xyz) * initialColor.xyz;
    vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * initialColor.xyz * max(0.0, dot(L, N));
    vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(0.0, dot(R, V)), material.shininess);
    
    frag_color = vec4(ambient + diffuse + specular, 1.0);
}