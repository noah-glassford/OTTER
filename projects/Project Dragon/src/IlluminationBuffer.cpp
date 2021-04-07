#include "IlluminationBuffer.h"
#include <RenderingManager.h>
#include <Transform.h>
#include <BackendHandler.h>
int LightingToggle = 1;
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
	//_pointLights.AllocateMemory(sizeof(LightSource) * _numLights);
	if (_sunEnabled) {
		_sunBuffer.SendData(reinterpret_cast<void*>(&_sun), sizeof(DirectionalLight));
	}

	BackendHandler::imGuiCallbacks.push_back([&]() {
		if (ImGui::Button("Toggle Lights"))
		{
			if (LightingToggle == 1)
				LightingToggle = 0;
			else if (LightingToggle == 0)
				LightingToggle = 1;
		}
		});


	PostEffect::Init(width, height);
}
int LightCount = 0;
void IlluminationBuffer::ApplyEffect(GBuffer* gBuffer)
{
	


	

	if (LightingToggle)
	{
		_sunBuffer.SendData(reinterpret_cast<void*>(&_sun), sizeof(DirectionalLight));
		if (_sunEnabled) {
			_shaders[Lights::DIRECTIONAL]->Bind();
			_shaders[Lights::DIRECTIONAL]->SetUniformMatrix("u_LightSpaceMatrix", _lightSpaceViewProj);
			_shaders[Lights::DIRECTIONAL]->SetUniform("u_CamPos", _camPos);

			LightCount = 0;

			RenderingManager::activeScene->Registry().view<Transform, LightSource>().each([&](entt::entity entity, Transform& t, LightSource& l) {
				LightCount++;
				//create the string to send to the shader
				if (LightCount <= 99)
				{
					std::string uniformName;
					uniformName = "PointLights[";
					uniformName += std::to_string(LightCount);
					uniformName += "].";
					//std::cout << "Light Count on frame: " << LightCount << std::endl;
					//std::cout << "Light Position" << t.GetLocalPosition().x << " " << t.GetLocalPosition().y << " " << t.GetLocalPosition().z << " \n";
					_shaders[Lights::DIRECTIONAL]->SetUniform(uniformName + "position", t.GetLocalPosition());
					_shaders[Lights::DIRECTIONAL]->SetUniform(uniformName + "ambient", l.m_Ambient);
					_shaders[Lights::DIRECTIONAL]->SetUniform(uniformName + "diffuse", l.m_Diffuse);
					_shaders[Lights::DIRECTIONAL]->SetUniform(uniformName + "specular", l.m_Specular);

				}

				});
			_shaders[Lights::DIRECTIONAL]->SetUniform("u_NumLights", LightCount);

			//_shaders[Lights::DIRECTIONAL]->SetUniform("u_LightingToggle", LightingToggle);

			_sunBuffer.Bind(0);



			//_pointLights.Bind(1);
			gBuffer->BindLighting();

			_buffers[1]->RenderToFSQ();

			gBuffer->UnbindLighting();

			_sunBuffer.Unbind(0);
			//_pointLights.Unbind(1);



			_shaders[Lights::DIRECTIONAL]->UnBind();
		}
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

void IlluminationBuffer::SetPointLights(UniformBuffer pl)
{
	_pointLights = pl;
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
