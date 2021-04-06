#pragma once

#include "PostEffect.h"
#include "UniformBuffer.h"
#include "GBuffer.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "LightSource.h"

enum Lights
{
	DIRECTIONAL,
	AMBIENT
};

//This is a post effect to make our job easier
class IlluminationBuffer : public PostEffect
{
public:
	IlluminationBuffer(unsigned numLights)
	{
		_numLights = numLights;
	}

	IlluminationBuffer() = delete;

	//Initializes framebuffer
	//Overrides post effect Init
	void Init(unsigned width, unsigned height) override;
	
	//Makes it so apply effect with a PostEffect does nothing for this object
	void ApplyEffect(PostEffect* buffer) override { };
	//Can only apply effect using GBuffer object
	void ApplyEffect(GBuffer* gBuffer);

	void DrawIllumBuffer();

	void SetLightSpaceViewProj(glm::mat4 lightSpaceViewProj);
	void SetCamPos(glm::vec3 camPos);

	void SetPointLights(UniformBuffer pl);

	DirectionalLight& GetSunRef();
	
	//Sets the sun in the scene
	void SetSun(DirectionalLight newSun);
	void SetSun(glm::vec4 lightDir, glm::vec4 lightCol);

	void EnableSun(bool enabled);

	std::vector<Shader::sptr> GetShaders()
	{
		return _shaders;
	}

private:
	glm::mat4 _lightSpaceViewProj;
	glm::vec3 _camPos;

	UniformBuffer _sunBuffer;
	UniformBuffer _pointLights;

	bool _sunEnabled = true;
	unsigned _numLights = 5;
	DirectionalLight _sun;
};