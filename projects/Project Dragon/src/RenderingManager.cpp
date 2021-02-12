#include "RenderingManager.h"
#include <RendererComponent.h>
#include <Transform.h>
#include "Framebuffer.h"
#include "BackendHandler.h"
#include <GreyscaleEffect.h>
#include <ColorCorrection.h>
#include <Player.h>
#include <Enemy.h>
Shader::sptr RenderingManager::BaseShader = NULL;
Shader::sptr RenderingManager::NoOutline = NULL;
Shader::sptr RenderingManager::SkyBox = NULL;
Shader::sptr RenderingManager::Passthrough = NULL;
GameScene::sptr RenderingManager::activeScene;
void RenderingManager::Init()
{

	// GL states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL); // New 


	BaseShader = Shader::Create();
	//First we initialize our shaders
	BaseShader->LoadShaderPartFromFile("shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	BaseShader->LoadShaderPartFromFile("shader/Cel_Shaded_Outline.glsl", GL_FRAGMENT_SHADER);
	BaseShader->Link();

	NoOutline = Shader::Create();
	//First we initialize our shaders
	NoOutline->LoadShaderPartFromFile("shader/vertex_shader.glsl", GL_VERTEX_SHADER);
	NoOutline->LoadShaderPartFromFile("shader/Cel_Shaded_No_Outline.glsl", GL_FRAGMENT_SHADER);
	NoOutline->Link();


	//then we set some base values
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 lightCol = glm::vec3(0.9f, 0.85f, 0.5f);
	float     lightAmbientPow = 0.05f;
	float     lightSpecularPow = 1.0f;
	glm::vec3 ambientCol = glm::vec3(1.0f);
	float     ambientPow = 0.6f;
	float     lightLinearFalloff = 0.09f;
	float     lightQuadraticFalloff = 0.032f;

	// every frame
	BaseShader->SetUniform("u_LightPos", lightPos);
	BaseShader->SetUniform("u_LightCol", lightCol);
	BaseShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
	BaseShader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
	BaseShader->SetUniform("u_AmbientCol", ambientCol);
	BaseShader->SetUniform("u_AmbientStrength", ambientPow);
	BaseShader->SetUniform("u_LightAttenuationConstant", 1.0f);
	BaseShader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
	BaseShader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);

	// every frame
	NoOutline->SetUniform("u_LightPos", lightPos);
	NoOutline->SetUniform("u_LightCol", lightCol);
	NoOutline->SetUniform("u_AmbientLightStrength", lightAmbientPow);
	NoOutline->SetUniform("u_SpecularLightStrength", lightSpecularPow);
	NoOutline->SetUniform("u_AmbientCol", ambientCol);
	NoOutline->SetUniform("u_AmbientStrength", ambientPow);
	NoOutline->SetUniform("u_LightAttenuationConstant", 1.0f);
	NoOutline->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
	NoOutline->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);

	//creates some IMGUI sliders
	BackendHandler::imGuiCallbacks.push_back([&]() {
		if (ImGui::CollapsingHeader("Scene Level Lighting Settings"))
		{
			if (ImGui::ColorPicker3("Ambient Color", glm::value_ptr(ambientCol))) {
				BaseShader->SetUniform("u_AmbientCol", ambientCol);
			}
			if (ImGui::SliderFloat("Fixed Ambient Power", &ambientPow, 0.01f, 1.0f)) {
				BaseShader->SetUniform("u_AmbientStrength", ambientPow);
			}
		}
		if (ImGui::CollapsingHeader("Light Level Lighting Settings"))
		{
			if (ImGui::DragFloat3("Light Pos", glm::value_ptr(lightPos), 0.01f, -10.0f, 10.0f)) {
				BaseShader->SetUniform("u_LightPos", lightPos);
			}
			if (ImGui::ColorPicker3("Light Col", glm::value_ptr(lightCol))) {
				BaseShader->SetUniform("u_LightCol", lightCol);
			}
			if (ImGui::SliderFloat("Light Ambient Power", &lightAmbientPow, 0.0f, 1.0f)) {
				BaseShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
			}
			if (ImGui::SliderFloat("Light Specular Power", &lightSpecularPow, 0.0f, 1.0f)) {
				BaseShader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
			}
			if (ImGui::DragFloat("Light Linear Falloff", &lightLinearFalloff, 0.01f, 0.0f, 1.0f)) {
				BaseShader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
			}
			if (ImGui::DragFloat("Light Quadratic Falloff", &lightQuadraticFalloff, 0.01f, 0.0f, 1.0f)) {
				BaseShader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);
			}
		}
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

void RenderingManager::Render()
{
	//gets frame buffer from the active scene
	PostEffect* postEffect;
	GreyscaleEffect* greyscale;
	ColorCorrectionEffect* colEffect;
	postEffect = &activeScene->FindFirst("Basic Effect").get<PostEffect>();
	greyscale = &activeScene->FindFirst("Greyscale Effect").get<GreyscaleEffect>();
	colEffect = &activeScene->FindFirst("ColorGrading Effect").get<ColorCorrectionEffect>();

	// Clear the screen
	
	//greyscale->Clear();
	postEffect->Clear();
	colEffect->Clear();

	glClearColor(0.08f, 0.17f, 0.31f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update all world matrices for this frame
	activeScene->Registry().view<Transform>().each([](entt::entity entity, Transform& t) {
		t.UpdateWorldMatrix();
		});

	// Update all world enemies for this frame
	activeScene->Registry().view<Enemy, PhysicsBody>().each([](entt::entity entity, Enemy& e, PhysicsBody& p) {
		e.Update(p);
		if (e.m_hp <= 0)
		{
			btTransform t;
			t.setOrigin(btVector3(0, 0, -1000));
			p.GetBody()->setCenterOfMassTransform(t);
		}
		});


	//get the camera mat4s
	Transform& camTransform = activeScene->FindFirst("Camera").get<Transform>();
	activeScene->FindFirst("Camera").get<Player>().Update();

	
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
		

		//update the player class
		



		//greyscale->ApplyEffect(postEffect);
		//greyscale->DrawToScreen();
		colEffect->ApplyEffect(postEffect);
		colEffect->DrawToScreen();
		

		postEffect->UnBindBuffer();

		BackendHandler::RenderImGui();

		activeScene->Poll();
		glfwSwapBuffers(BackendHandler::window);

	


	


}
