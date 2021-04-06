#include "GameScene.h"
#include "PhysicsSystem.h"
#include "Timer.h"
#include <AssetLoader.h>
#include <MainMenu.h>
#include <DeathScreen.h>
#include <ft2build.h>
#include FT_FREETYPE_H
int main() 
{ 
	BackendHandler::InitAll();
	PhysicsSystem::Init();
	AssetLoader::Init();


	int ref;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &ref);
	std::cout << "Max uniform components: " << ref << "\n";

	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &ref);
	std::cout << "Max uniform block size: " << ref << "\n";

	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &ref);
	std::cout << "Max uniform blocks: " << ref << "\n";

	BackendHandler::m_Scenes.push_back(new MainMenuScene());
	BackendHandler::m_Scenes.push_back(new MainGameScene());
	BackendHandler::m_Scenes.push_back(new DeathSceen());
	BackendHandler::m_Scenes[1]->InitGameScene();
	BackendHandler::m_ActiveScene = 1;

	RenderingManager::InitPostEffects();

	


	//return;

	///// Game loop /////
	while (!glfwWindowShouldClose(BackendHandler::window)) {
		glfwPollEvents();
		Timer::Tick();
		BackendHandler::UpdateInput();
		BackendHandler::UpdateAudio();
		PhysicsSystem::Update();
		
		RenderingManager::Render();
	
		
		
	
		
	}
	BackendHandler::ShutdownImGui();
	
	return 0; 
} 
