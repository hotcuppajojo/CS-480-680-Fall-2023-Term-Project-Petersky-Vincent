#version 460

smooth in vec3 color;
layout (binding=0) uniform sampler2D sp;
in vec2 tc;
uniform bool hasTexture;

out vec4 frag_color;

void main(void)
{
    if(hasTexture)
        frag_color = texture(sp,tc);
    else 
        frag_color = vec4(color.rgb, 1.0);
}