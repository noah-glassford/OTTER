#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outUV;

uniform vec2 u_Scale;
uniform vec2 u_Offset;
uniform int u_IsMenu;

void main()
{
	vec4 scaled = vec4(inPosition.x * u_Scale.x, -inPosition.z, inPosition.z * 0.1, 1.0);
	gl_Position = vec4(scaled.x+u_Offset.x, scaled.y+u_Offset.y, scaled.zw);

	outPos = gl_Position.xyz;

	outNormal = inNormal;

	outColor = vec3(1.0,1.0,1.0);

	
	outUV = inUV;

}