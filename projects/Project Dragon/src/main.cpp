#include "GameScene.h"
#include "PhysicsSystem.h"
#include "Timer.h"
#include <AssetLoader.h>
#include <ft2build.h>
#include FT_FREETYPE_H
int main() 
{ 
	BackendHandler::InitAll();
	PhysicsSystem::Init();
	AssetLoader::Init();
	MainGameScene maingame;
	maingame.InitGameScene();

	

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
