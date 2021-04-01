#version 420

layout(location = 0) in vec2 inUV;

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(binding = 30) uniform sampler2D s_ShadowMap;

layout (binding = 0) uniform sampler2D s_albedoTex;
layout (binding = 1) uniform sampler2D s_normalsTex;
layout (binding = 2) uniform sampler2D s_specularTex;
layout (binding = 3) uniform sampler2D s_positionTex;

layout (binding = 4) uniform sampler2D s_lightAccumTex;

uniform vec3 u_CamPos;

uniform PointLight pointLight;

out vec4 frag_color;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);




// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	
    vec4 textureColor = texture(s_albedoTex, inUV);
	vec4 lightAccum = texture(s_lightAccumTex, inUV);
	//vec4 skybox = texture(s_skyBox, inUV);
   
   vec3 WorldPos = texture( s_positionTex, inUV).xyz;
    vec3 Color = texture(s_specularTex, inUV).xyz;
    vec3 Normal = texture(s_normalsTex, inUV).xyz;
    vec3 ViewDir = u_CamPos - WorldPos;

    frag_color = vec4((CalcPointLight(pointLight, Normal, WorldPos, ViewDir) * textureColor.rgb), 1);

    }



    vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.f);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(s_albedoTex, inUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(s_albedoTex, inUV));
    vec3 specular = light.specular * spec * vec3(texture(s_albedoTex, inUV));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

