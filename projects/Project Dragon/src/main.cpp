#include "GameScene.h"
#include "PhysicsSystem.h"
#include "Timer.h"
#include <AssetLoader.h>
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
