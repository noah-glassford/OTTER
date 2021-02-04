#include "RenderingManager.h"
#include <RendererComponent.h>
#include <Transform.h>
#include "Framebuffer.h"
#include "BackendHandler.h"
Shader::sptr RenderingManager::BaseShader = NULL;
Shader::sptr RenderingManager::SkyBox = NULL;
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
	BaseShader->LoadShaderPartFromFile("shader/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
	BaseShader->Link();

	//then we set some base values
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 lightCol = glm::vec3(0.9f, 0.85f, 0.5f);
	float     lightAmbientPow = 0.05f;
	float     lightSpecularPow = 1.0f;
	glm::vec3 ambientCol = glm::vec3(1.0f);
	float     ambientPow = 0.1f;
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

	SkyBox = Shader::Create();
	//Want to add a test skybox
	// Load our shaders

	SkyBox->LoadShaderPartFromFile("shader/skybox-shader.vert.glsl", GL_VERTEX_SHADER);
	SkyBox->LoadShaderPartFromFile("shader/skybox-shader.frag.glsl", GL_FRAGMENT_SHADER);
	SkyBox->Link();

}

void RenderingManager::Render()
{
	//gets frame buffer from the active scene
	Framebuffer* testBuffer;

	testBuffer = &activeScene->FindFirst("Basic Buffer").get<Framebuffer>();

	// Clear the screen
	testBuffer->Clear();

	glClearColor(0.08f, 0.17f, 0.31f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update all world matrices for this frame
	activeScene->Registry().view<Transform>().each([](entt::entity entity, Transform& t) {
		t.UpdateWorldMatrix();
		});

	//get the camera mat4s
	Transform& camTransform = activeScene->FindFirst("Camera").get<Transform>();
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

	testBuffer->Bind();

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
		testBuffer->Unbind();

		testBuffer->DrawToBackbuffer();

		activeScene->Poll();
		glfwSwapBuffers(BackendHandler::window);




	Application::Instance().ActiveScene = nullptr;


}
