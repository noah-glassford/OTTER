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
#include <MorphAnimator.h>
Shader::sptr RenderingManager::BaseShader = NULL;
Shader::sptr RenderingManager::NoOutline = NULL;
Shader::sptr RenderingManager::SkyBox = NULL;
Shader::sptr RenderingManager::Passthrough = NULL;
Shader::sptr RenderingManager::AnimationShader = NULL;
Shader::sptr RenderingManager::UIShader = NULL;
	
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
	BaseShader->LoadShaderPartFromFile("shader/Multiple_Light_Outline.glsl", GL_FRAGMENT_SHADER);
	BaseShader->Link();

	NoOutline = Shader::Create();
	//First we initialize our shaders
	NoOutline->LoadShaderPartFromFile("shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	NoOutline->LoadShaderPartFromFile("shader/Multiple_Light_NoOutline.glsl", GL_FRAGMENT_SHADER);
	NoOutline->Link();

	

	UIShader = Shader::Create();
	UIShader->LoadShaderPartFromFile("shader/ui_vert.glsl", GL_VERTEX_SHADER);
	UIShader->LoadShaderPartFromFile("shader/ui_frag.glsl", GL_FRAGMENT_SHADER);
	UIShader->Link();
	UIShader->SetUniform("u_Scale", glm::vec2(1, 0.1));
	UIShader->SetUniform("u_Offset", glm::vec2(0, 0));

	BaseShader->SetUniform("u_LightAttenuationConstant", 1.f);
	BaseShader->SetUniform("u_LightAttenuationLinear", 0.08f);
	BaseShader->SetUniform("u_LightAttenuationQuadratic", 0.032f);



	//init attenuation
	NoOutline->SetUniform("u_LightAttenuationConstant", 1.f);
	NoOutline->SetUniform("u_LightAttenuationLinear", 0.08f);
	NoOutline->SetUniform("u_LightAttenuationQuadratic", 0.032f);


	//initialize primary fragment shader DirLight & spotlight
	BaseShader->SetUniform("dirLight.direction", glm::vec3(-0.0f, -0.0f, -1.0f));
	BaseShader->SetUniform("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	BaseShader->SetUniform("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	BaseShader->SetUniform("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

	//initialize primary fragment shader DirLight & spotlight
	NoOutline->SetUniform("dirLight.direction", glm::vec3(-0.0f, -0.0f, -1.0f));
	NoOutline->SetUniform("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
	NoOutline->SetUniform("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	NoOutline->SetUniform("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));


	

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
void RenderingManager::Render()
{
	
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

	int enemyCount = 0;
	// Update all world enemies for this frame
	activeScene->Registry().view<Enemy, PhysicsBody, Transform>().each([](entt::entity entity, Enemy& e, PhysicsBody& p, Transform& t) {
		
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
		//	btTransform t;
			//t.setOrigin(btVector3(0, 0, -1000));
			//p.GetBody()->setCenterOfMassTransform(t);
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

	

	postEffect->BindBuffer(0);

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
		BackendHandler::RenderVAO(renderer.Material->Shader, renderer.Mesh, viewProjection, transform);
		});
		
		
		bloomEffect->ApplyEffect(postEffect);
		bloomEffect->DrawToScreen();
		colEffect->ApplyEffect(postEffect);
		colEffect->DrawToScreen();

		postEffect->UnBindBuffer();
		
		BackendHandler::RenderImGui();

		activeScene->Poll();
		glfwSwapBuffers(BackendHandler::window);

		


	


}
