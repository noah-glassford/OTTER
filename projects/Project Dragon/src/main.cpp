#include "GameScene.h"
#include "PhysicsSystem.h"
int main() 
{ 
	BackendHandler::InitAll();
	PhysicsSystem::Init();
	MainGameScene maingame;
	maingame.InitGameScene();

	///// Game loop /////
	while (!glfwWindowShouldClose(BackendHandler::window)) {
		glfwPollEvents();
		PhysicsSystem::Update();
		RenderingManager::Render();
		BackendHandler::RenderImGui();
		
	}
	BackendHandler::ShutdownImGui();
	
	return 0; 
} 
