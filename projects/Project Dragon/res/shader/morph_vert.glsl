#version 410

//Keyframe 1
layout(location = 0) in vec3 FramePos1;
layout(location = 1) in vec3 Color1;
layout(location = 2) in vec3 inNormal1;
layout(location = 3) in vec2 inUV1;

//Keyframe 2
layout(location = 4) in vec3 FramePos2;
layout(location = 5) in vec3 Color2;
layout(location = 6) in vec3 inNormal2;

//We only output the result from the lerp between frame 1 & 2
layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outUV;

uniform mat4 u_ModelViewProjection;
uniform mat4 u_View;
uniform mat4 u_Model;
//uniform mat3 u_ModelRotation;
uniform vec3 u_LightPos;

//for interpolation
uniform float t;


void main() 
{
	//vec3 mixedPos;
	//mixedPos = mix(FramePos1, FramePos2, t);
	outPos = mix(FramePos1, FramePos2, t);
//	vec3 mixedNorms;
//	mixedNorms =
	outNormal = (u_Model * vec4(mix(inNormal1, inNormal2, t),1)).xyz;
	outUV = inUV1;
	outColor = Color1;
	gl_Position = u_ModelViewProjection * vec4(outPos,1.0);
	
}
