#include "GameScene.h"
#include "PhysicsSystem.h"
#include "Timer.h"
int main() 
{ 
	BackendHandler::InitAll();
	PhysicsSystem::Init();
	MainGameScene maingame;
	maingame.InitGameScene();

	///// Game loop /////
	while (!glfwWindowShouldClose(BackendHandler::window)) {
		glfwPollEvents();
		Timer::Tick();
		BackendHandler::UpdateInput();
		PhysicsSystem::Update();
		
		RenderingManager::Render();
		BackendHandler::RenderImGui();
		
		
	
		
	}
	BackendHandler::ShutdownImGui();
	
	return 0; 
} 
