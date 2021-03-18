#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;
out vec3 color;

uniform sampler2D s_Specular;
uniform sampler2D s_Diffuse;
uniform vec3 spriteColor;

void main()
{    

    vec3 Full = vec3(texture(s_Specular, inUV));
    vec3 Empty = vec3(texture(s_Diffuse, inUV));
    vec3 Mixed = mix(Full, Empty, 0);
    if (Mixed.rgb == vec3(0.0,1.0,0.0))
      discard; 
      color = Mixed;
}