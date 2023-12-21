// fragment_light_shader.glsl

#version 460

#define MAX_LIGHTS 10

struct Lights {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 positionViewSpace;
};

uniform int numLights;
uniform Lights lights[MAX_LIGHTS];
uniform vec4 globalAmbient;

layout (binding=0) uniform sampler2D sampler;

uniform bool lightHasTexture;
uniform bool lightHasTC;

in vec3 colorOut;
in vec2 tcOut;
in vec3 normalViewSpace;
in vec3 fragPosViewSpace;
out vec4 frag_color;

void main(void) {
    vec3 viewDir = normalize(-fragPosViewSpace);
    vec3 resultColor = globalAmbient.rgb * colorOut;

    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(lights[i].positionViewSpace - fragPosViewSpace);
        vec3 halfDir = normalize(lightDir + viewDir);
        float specIntensity = pow(max(dot(normalViewSpace, halfDir), 0.0), 16.0);

        vec3 ambient = lights[i].ambient.rgb * colorOut;
        vec3 diffuse = lights[i].diffuse.rgb * max(dot(normalViewSpace, lightDir), 0.0) * colorOut;
        vec3 specular = lights[i].specular.rgb * specIntensity;

        resultColor += ambient + diffuse + specular;
    }

    if (lightHasTexture) {
        frag_color = texture(sampler, tcOut) * vec4(resultColor, 1.0);
    } else {
        frag_color = vec4(resultColor, 1.0);
    }
}