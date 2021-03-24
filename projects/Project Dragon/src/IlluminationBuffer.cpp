#include "IlluminationBuffer.h"

void IlluminationBuffer::Init(unsigned width, unsigned height)
{
	int index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);

	index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);

	index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);

	//Loads the directional gBuffer shader
	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shader/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shader/gBuffer_directional_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();

	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shader/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shader/gBuffer_ambient_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();

	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shader/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shader/gBuffer_point_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();

	_sunBuffer.AllocateMemory(sizeof(DirectionalLight));

	if (_sunEnabled) {
		_sunBuffer.SendData(reinterpret_cast<void*>(&_sun), sizeof(DirectionalLight));
	}

	PostEffect::Init(width, height);
}

void IlluminationBuffer::ApplyEffect(GBuffer* gBuffer)
{
	_sunBuffer.SendData(reinterpret_cast<void*>(&_sun), sizeof(DirectionalLight));
	if (_sunEnabled) {
		_shaders[Lights::DIRECTIONAL]->Bind();
		_shaders[Lights::DIRECTIONAL]->SetUniformMatrix("u_LightSpaceMatrix", _lightSpaceViewProj);
		_shaders[Lights::DIRECTIONAL]->SetUniform("u_CamPos", _camPos);

		_sunBuffer.Bind(0);

		gBuffer->BindLighting();

		_buffers[1]->RenderToFSQ();

		gBuffer->UnbindLighting();

		_sunBuffer.Unbind(0);
		_shaders[Lights::DIRECTIONAL]->UnBind();
	}

	//Bind ambient shader
	_shaders[Lights::AMBIENT]->Bind();

	_sunBuffer.Bind(0);

	gBuffer->BindLighting();
	_buffers[1]->BindColorAsTexture(0, 4);
	_buffers[0]->BindColorAsTexture(0, 5);

	_buffers[0]->RenderToFSQ();

	_buffers[0]->UnbindTexture(5);
	_buffers[1]->UnbindTexture(4);


	gBuffer->UnbindLighting();

	_sunBuffer.Unbind(0);

	_shaders[Lights::AMBIENT]->UnBind();


	//Bind ambient shader
	_shaders[2]->Bind();
	_shaders[2]->SetUniform("u_CamPos", _camPos);
	std::string uniformName;
	uniformName = "pointLight.";
	//this will be the begining, now we just need to add the part of the struct we want to set
	_shaders[2]->SetUniform(uniformName + "position", glm::vec3(2, 2, 2));
	_shaders[2]->SetUniform(uniformName + "ambient", glm::vec3(1, 1, 1));
	_shaders[2]->SetUniform(uniformName + "diffuse", glm::vec3(1, 1, 1));
	_shaders[2]->SetUniform(uniformName + "specular", glm::vec3(1, 1, 1));

	_sunBuffer.Bind(0);


	gBuffer->BindLighting();
	_buffers[2]->BindColorAsTexture(0, 4);
	_buffers[1]->BindColorAsTexture(0, 5);

	_buffers[2]->RenderToFSQ();

	_buffers[1]->UnbindTexture(5);
	_buffers[2]->UnbindTexture(4);


	gBuffer->UnbindLighting();

	_sunBuffer.Unbind(0);

	_shaders[2]->UnBind();

}

void IlluminationBuffer::DrawIllumBuffer()
{
	_shaders[_shaders.size() - 1]->Bind();

	_buffers[2]->BindColorAsTexture(0, 0);
	Framebuffer::DrawFullscreenQuad;
	_buffers[2]->UnbindTexture(0);

	_shaders[_shaders.size() - 1]->UnBind();
}

void IlluminationBuffer::SetLightSpaceViewProj(glm::mat4 lightSpaceViewProj)
{
	_lightSpaceViewProj = lightSpaceViewProj;
}

void IlluminationBuffer::SetCamPos(glm::vec3 camPos)
{
	_camPos = camPos;
}

DirectionalLight& IlluminationBuffer::GetSunRef()
{
	return _sun;
}

void IlluminationBuffer::SetSun(DirectionalLight newSun)
{
	_sun = newSun;
}

void IlluminationBuffer::SetSun(glm::vec4 lightDir, glm::vec4 lightCol)
{
	_sun._lightDirection = lightDir;
	_sun._lightCol = lightCol;
}

void IlluminationBuffer::EnableSun(bool enabled)
{
	_sunEnabled = enabled;
}
