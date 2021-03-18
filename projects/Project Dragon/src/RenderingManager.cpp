#include "RenderingManager.h"
#include <RendererComponent.h>
#include <Transform.h>
#include "Framebuffer.h"
#include "BackendHandler.h"
#include <GreyscaleEffect.h>
#include <ColorCorrection.h>
#include <Player.h>
#include <Enemy.h>
#include <AudioEngine.h>
#include <Bloom.h>
#include <LightSource.h>

#include <UI.h>
#include <Interpolation.h>
#include <PhysicsSystem.h>
#include <CameraControlBehaviour.h>
#include <WorldBuilderV2.h>
#include <UniformBuffer.h>
#include <DirectionalLight.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <SkinnedMesh.h>

Shader::sptr RenderingManager::BaseShader = NULL;
Shader::sptr RenderingManager::NoOutline = NULL;
Shader::sptr RenderingManager::SkyBox = NULL;
Shader::sptr RenderingManager::Passthrough = NULL;
Shader::sptr RenderingManager::UIShader = NULL;
Shader::sptr RenderingManager::simpleDepthShader = NULL;
Shader::sptr RenderingManager::BoneAnimShader = NULL;
	
GameScene::sptr RenderingManager::activeScene;

bool ShouldBloom;
bool TextureToggle;
int NumPasses;
float Threshold;

void RenderingManager::Init()
{
	// GL states
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL); // New 


	BaseShader = Shader::Create();
	//First we initialize our shaders
	BaseShader->LoadShaderPartFromFile("shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	BaseShader->LoadShaderPartFromFile("shader/directional_blinn_phong_frag.glsl", GL_FRAGMENT_SHADER);
	BaseShader->Link();

	NoOutline = Shader::Create();
	//First we initialize our shaders
	NoOutline->LoadShaderPartFromFile("shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	NoOutline->LoadShaderPartFromFile("shader/directional_blinn_phong_frag.glsl", GL_FRAGMENT_SHADER);
	NoOutline->Link();

	simpleDepthShader = Shader::Create();
	//First we initialize our shaders
	simpleDepthShader->LoadShaderPartFromFile("shader/simple_depth_vert.glsl", GL_VERTEX_SHADER);
	simpleDepthShader->LoadShaderPartFromFile("shader/simple_depth_frag.glsl", GL_FRAGMENT_SHADER);
	simpleDepthShader->Link();

	//For animated models only
	BoneAnimShader = Shader::Create();
	//First we initialize our shaders
	BoneAnimShader->LoadShaderPartFromFile("shader/skinned_vertex_shader.glsl", GL_VERTEX_SHADER);
	BoneAnimShader->LoadShaderPartFromFile("shader/directional_blinn_phong_frag.glsl", GL_FRAGMENT_SHADER);
	BoneAnimShader->Link();

	

	UIShader = Shader::Create();
	UIShader->LoadShaderPartFromFile("shader/ui_vert.glsl", GL_VERTEX_SHADER);
	UIShader->LoadShaderPartFromFile("shader/ui_frag.glsl", GL_FRAGMENT_SHADER);
	UIShader->Link();
	UIShader->SetUniform("u_Scale", glm::vec2(1, 0.1));
	UIShader->SetUniform("u_Offset", glm::vec2(0, 0));

	BaseShader->SetUniform("u_LightAttenuationConstant", 1.f);
	BaseShader->SetUniform("u_LightAttenuationLinear", 0.08f);
	BaseShader->SetUniform("u_LightAttenuationQuadratic", 0.032f);

	BoneAnimShader->SetUniform("u_LightAttenuationConstant", 1.f);
	BoneAnimShader->SetUniform("u_LightAttenuationLinear", 0.08f);
	BoneAnimShader->SetUniform("u_LightAttenuationQuadratic", 0.032f);



	//init attenuation
	NoOutline->SetUniform("u_LightAttenuationConstant", 1.f);
	NoOutline->SetUniform("u_LightAttenuationLinear", 0.08f);
	NoOutline->SetUniform("u_LightAttenuationQuadratic", 0.032f);


	


	

	//creates some IMGUI sliders
	BackendHandler::imGuiCallbacks.push_back([&]() {
		
		});

	SkyBox = std::make_shared<Shader>();
	//Want to add a test skybox
	// Load our shaders

	SkyBox->LoadShaderPartFromFile("shader/skybox-shader.vert.glsl", GL_VERTEX_SHADER);
	SkyBox->LoadShaderPartFromFile("shader/skybox-shader.frag.glsl", GL_FRAGMENT_SHADER);
	SkyBox->Link();

	Passthrough = std::make_shared<Shader>();

	Passthrough->LoadShaderPartFromFile("shader/passthrough_vert.glsl", GL_VERTEX_SHADER);
	Passthrough->LoadShaderPartFromFile("shader/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
	Passthrough->Link();

}
bool DeathSoundPlayed = false;
int LightCount;
int enemyCount = 0;
void RenderingManager::Render()
{
	//Framebuffer* shadowBuf = &activeScene->FindFirst("Shadow Buffer").get<Framebuffer>();
	PostEffect* postEffect = &activeScene->FindFirst("Basic Effect").get<PostEffect>();
	BloomEffect* bloomEffect = &activeScene->FindFirst("Bloom Effect").get<BloomEffect>();
	ColorCorrectionEffect* colEffect = &activeScene->FindFirst("ColorGrading Effect").get<ColorCorrectionEffect>();;


	postEffect->Clear();
	bloomEffect->Clear();
	colEffect->Clear();

	glClearColor(0.08f, 0.17f, 0.31f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update all world matrices for this frame
	activeScene->Registry().view<Transform>().each([](entt::entity entity, Transform& t) {
		t.UpdateWorldMatrix();
		});

	glm::mat4 lightProjectionMatrix = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -30.0f, 30.0f);
	DirectionalLight& sun = activeScene->FindFirst("SUN").get<DirectionalLight>();
	glm::vec3 LightDirection = sun._lightDirection;
	glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(LightDirection), glm::vec3(), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 lightSpaceViewProj = lightProjectionMatrix * lightViewMatrix;


	enemyCount = 0;
	// Update all world enemies for this frame
	activeScene->Registry().view<Enemy, PhysicsBody, Transform>().each([](entt::entity entity, Enemy& e, PhysicsBody& p, Transform& t) {
		enemyCount++;
		e.Update(p);
		if (e.m_hp <= 0.f)
		{
			activeScene->Registry().destroy(entity);

			//t.SetLocalPosition(0,0,-1000);
			//play temp death sound
			//Placeholder shoot sfx
			AudioEngine& engine = AudioEngine::Instance();

			AudioEvent& tempEnDeath = engine.GetEvent("Level Complete");
			if (!DeathSoundPlayed)
			{
				DeathSoundPlayed = true;
				tempEnDeath.Play();
			}
			

			
		}

		});
	
	LightCount = 0;
	activeScene->Registry().view<Transform, LightSource>().each([](entt::entity entity, Transform& t, LightSource& l) {
		
		if (LightCount <= 50)
		{
			//create the string to send to the shader
			std::string uniformName;
			uniformName = "pointLights[";
			uniformName += std::to_string(LightCount);
			uniformName += "].";
			//this will be the begining, now we just need to add the part of the struct we want to set
			BaseShader->SetUniform(uniformName + "position", t.GetLocalPosition());
			BaseShader->SetUniform(uniformName + "ambient", l.m_Ambient);
			BaseShader->SetUniform(uniformName + "diffuse", l.m_Diffuse);
			BaseShader->SetUniform(uniformName + "specular", l.m_Specular);
			
			NoOutline->SetUniform(uniformName + "position", t.GetLocalPosition());
			NoOutline->SetUniform(uniformName + "ambient", l.m_Ambient);
			NoOutline->SetUniform(uniformName + "diffuse", l.m_Diffuse);
			NoOutline->SetUniform(uniformName + "specular", l.m_Specular);
		
		}
		LightCount++;
	
		});
	NoOutline->SetUniform("u_LightCount", LightCount);
	BaseShader->SetUniform("u_LightCount", LightCount);


	//get the camera mat4s
	Transform& camTransform = activeScene->FindFirst("Camera").get<Transform>();
	activeScene->FindFirst("Camera").get<Player>().Update();
	//temp
	//activeScene->FindFirst("NumberPlane").get<Transform>().LookAt(camTransform.GetLocalPosition());

	
	glm::mat4 view = glm::inverse(camTransform.LocalTransform());
	glm::mat4 projection = activeScene->FindFirst("Camera").get<Camera>().GetProjection();
	glm::mat4 viewProjection = projection * view;


	entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Transform>, RendererComponent> renderGroup =
		activeScene->Registry().group<RendererComponent>(entt::get_t<Transform>());



	renderGroup.sort<RendererComponent>([](const RendererComponent& l, const RendererComponent& r) {
		// Sort by render layer first, higher numbers get drawn last
		if (l.Material->RenderLayer < r.Material->RenderLayer) return true;
		if (l.Material->RenderLayer > r.Material->RenderLayer) return false;

		// Sort by shader pointer next (so materials using the same shader run sequentially where possible)
		if (l.Material->Shader < r.Material->Shader) return true;
		if (l.Material->Shader > r.Material->Shader) return false;

		// Sort by material pointer last (so we can minimize switching between materials)
		if (l.Material < r.Material) return true;
		if (l.Material > r.Material) return false;

		return false;
		});

	// Start by assuming no shader or material is applied
	Shader::sptr current = nullptr;
	ShaderMaterial::sptr currentMat = nullptr;



	//sets the scale for player HP Bar
	if(BackendHandler::m_ActiveScene == 1)
	{
		float t2 = 0.2 * activeScene->FindFirst("Camera").get<Player>().m_Hp;

	

		float scaleX = Interpolation::LERP(0, 1, t2);

		activeScene->FindFirst("PlayerHPBar").get<UI>().scale = glm::vec2(scaleX, 1);
	}

	activeScene->Registry().view<Transform, UI, RendererComponent>().each([](entt::entity entity, Transform& t, UI& ui, RendererComponent& rc)
		{
			UIShader->SetUniform("u_Scale", ui.scale);
			UIShader->SetUniform("u_Offset", ui.offset);
		});


	postEffect->BindBuffer(0);


	//firstly render gltf animations
	activeScene->Registry().view<GLTFSkinnedMesh, Transform>().each([](entt::entity entity, GLTFSkinnedMesh& m, Transform& t) {
		m.UpdateAnimation(m.GetAnimation(0), Timer::dt);
		Transform& camTransform = activeScene->FindFirst("Camera").get<Transform>();
		glm::mat4 view = glm::inverse(camTransform.LocalTransform());
		glm::mat4 projection = activeScene->FindFirst("Camera").get<Camera>().GetProjection();
		glm::mat4 viewProjection = projection * view;
		m.Draw(BoneAnimShader, viewProjection, (glm::mat4)t.LocalTransform());
		});


	// Iterate over the render group components and draw them
	renderGroup.each([&](entt::entity e, RendererComponent& renderer, Transform& transform) {
		// If the shader has changed, set up it's uniforms
		
		if (current != renderer.Material->Shader) {
			current = renderer.Material->Shader;
			current->Bind();
			BackendHandler::SetupShaderForFrame(current, view, projection);
		}

		// If the material has changed, apply it
		if (currentMat != renderer.Material) {
			currentMat = renderer.Material;
			currentMat->Apply();
		}

		
		//shadowBuf->BindDepthAsTexture(30);
		BackendHandler::RenderVAO(renderer.Material->Shader, renderer.Mesh, viewProjection, transform, lightSpaceViewProj);
		});
		
		
		
	//shadowBuf->UnbindTexture(30);

		postEffect->UnBindBuffer();

		bloomEffect->ApplyEffect(postEffect);
		bloomEffect->DrawToScreen();
		colEffect->ApplyEffect(postEffect);
		colEffect->DrawToScreen();

		
		BackendHandler::RenderImGui();

		activeScene->Poll();
		glfwSwapBuffers(BackendHandler::window);

		std::cout << enemyCount << std::endl;
		if (enemyCount <= 1 && BackendHandler::m_ActiveScene == 1)
		{
			activeScene->DeleteAllEnts();
			PhysicsSystem::ClearWorld();
			BackendHandler::m_Scenes[1]->InitGameScene();

			
		}


	


}
