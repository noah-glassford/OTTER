#version 410
/*
A large part of this shader is taken from https://learnopengl.com/Lighting/Multiple-lights
*/

//These 3 are basically the material
uniform sampler2D s_Diffuse;
uniform sampler2D s_Specular;
uniform float u_Shininess;

//for light toggle
uniform int u_Lightingtoggle;


uniform vec3  u_CamPos;

uniform int u_LightCount;

out vec4 frag_color;


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
//unsused for the time being, ill leave it in here for future use
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
//Set max number of lights to 50, required to be a constant
#define NR_POINT_LIGHTS 99


uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform SpotLight spotLight;

uniform float u_LightAttenuationConstant;
uniform float u_LightAttenuationLinear ;
uniform float u_LightAttenuationQuadratic;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;




// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


const int bands = 5;
const float scaleFactor = 1.0 / 5;


void main()
{
    vec3 norm = normalize(inNormal);
    vec3 viewDir = normalize(u_CamPos - inPos);

    vec3 N = normalize(inNormal);


    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalcDirLight(DirectionalLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < u_LightCount; i++)
        result += CalcPointLight(pointLights[i], norm, inPos, viewDir);    
    // phase 3: spot light
  //  result += CalcSpotLight(spotLight, norm, inPos, viewDir);    
    
      //result 

    frag_color = vec4(result, 1.0);

}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light._lightDirection);
    // diffuse shading
    float diff = max(dot(normal,  _lightDirection), 0.0);
       float level = floor(diff * bands);
    diff = level / bands;
    // specular shading
    vec3 reflectDir = reflect(-_lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

   
       
    ambient = dirLight._ambientCol * vec3(texture(s_Diffuse, inUV));
    diffuse = vec3(0.1f, 0.1f, 0.1f) * diff * vec3(texture(s_Diffuse, inUV));
    specular = vec3(0.1f, 0.1f, 0.1f) * spec * vec3(texture(s_Specular, inUV));

   
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    float level = floor(diff * bands);
    diff = level / bands;
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
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


// calculates the color when using a spot light.
/*
currently unused
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
     float level = floor(diff * bands);
    diff = level / bands;
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (u_LightAttenuationConstant + u_LightAttenuationLinear * distance + u_LightAttenuationQuadratic * (distance * distance)); 
    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
   
   ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    diffuse = floor(diffuse * bands)*scaleFactor;

    return (ambient + diffuse + specular);
}
*/
