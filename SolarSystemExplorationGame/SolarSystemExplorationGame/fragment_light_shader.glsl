#version 460

layout (binding=0) uniform sampler2D sampler;

uniform bool lightHasTexture;
uniform bool lightHasTC;

uniform mat4 lightProjection;
uniform mat4 lightView;
uniform mat4 lightModel;

smooth in vec3 color;
in vec2 tc;
out vec4 frag_color;

void main(void)
{
    if(lightHasTexture)
        frag_color = texture(sampler,tc);
    else 
        frag_color = vec4(color.rgb, 1.0);
}