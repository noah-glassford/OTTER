#version 420

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;
layout(location = 4) in vec4 inFragPosLightSpace;

struct DirectionalLight
{
	//Light direction (defaults to down, to the left, and a little forward)
	vec4 _lightDirection;

	//Generic Light controls
	vec4 _lightCol;

	//Ambience controls
	vec4 _ambientCol;
	float _ambientPow;
	
	//Power controls
	float _lightAmbientPow;
	float _lightSpecularPow;

	float _shadowBias;
};

struct PointLight {
    vec3 position;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (std140, binding = 0) uniform u_Lights
{
	DirectionalLight sun;
};

layout (binding = 30) uniform sampler2D s_ShadowMap;
uniform sampler2D s_Diffuse;
uniform sampler2D s_Diffuse2;
uniform sampler2D s_Specular;

#define NR_POINT_LIGHTS 99
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int u_LightCount;

uniform float u_TextureMix;
uniform vec3  u_CamPos;

uniform float u_LightAttenuationConstant;
uniform float u_LightAttenuationLinear ;
uniform float u_LightAttenuationQuadratic;



const int bands = 5;
const float scaleFactor = 1.0 / 5;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 frag_color;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	//Perspective division
	vec3 projectionCoordinates = fragPosLightSpace.xyz / fragPosLightSpace.w;

	//Transform into a [0,1] range
	projectionCoordinates = projectionCoordinates * 0.5 + 0.5;

	//Get the closest depth value
	float closestDepth = texture(s_ShadowMap, projectionCoordinates.xy).r;

	//Get the current depth according to our camera
	float currentDepth = projectionCoordinates.z;

	//Check whether there's a shadow
	float shadow = currentDepth - sun._shadowBias > closestDepth ? 1.0 : 0.0;

	return shadow; 
}

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	// Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(-sun._lightDirection.xyz);
	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = dif * sun._lightCol.xyz;// add diffuse intensity

	// Specular
	vec3 viewDir  = normalize(u_CamPos - inPos);
	vec3 h        = normalize(lightDir + viewDir);

	// Get the specular power from the specular map
	float texSpec = texture(s_Specular, inUV).x;
	float spec = pow(max(dot(N, h), 0.0), 4.0); // Shininess coefficient (can be a uniform)
	vec3 specular = sun._lightSpecularPow * texSpec * spec * sun._lightCol.xyz; // Can also use a specular color

	// Get the albedo from the diffuse / albedo map
	vec4 textureColor1 = texture(s_Diffuse, inUV);
	vec4 textureColor2 = texture(s_Diffuse2, inUV);
	vec4 textureColor = mix(textureColor1, textureColor2, u_TextureMix);

	float shadow = ShadowCalculation(inFragPosLightSpace);

	vec3 result = (
		(sun._ambientPow * sun._ambientCol.xyz) + // global ambient light
		(1.0 - shadow) * (diffuse + specular) // light factors from our single light
		) * inColor * textureColor.rgb; // Object color

		//Calculate our pointlights after
		 for(int i = 0; i < u_LightCount; i++)
        result += CalcPointLight(pointLights[i], N, inPos, viewDir);    

	frag_color = vec4(result, textureColor.a);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    float level = floor(diff * bands);
    diff = level / bands;
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5f);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (u_LightAttenuationConstant + u_LightAttenuationLinear * distance + u_LightAttenuationQuadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(s_Diffuse, inUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(s_Diffuse, inUV));
    vec3 specular = light.specular * spec * vec3(texture(s_Specular, inUV));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
