#include "GameScene.h"
int main() 
{ 
	BackendHandler::InitAll();
	MainGameScene maingame;
	maingame.InitGameScene();

	///// Game loop /////
	while (!glfwWindowShouldClose(BackendHandler::window)) {
		glfwPollEvents();
		RenderingManager::Render();
		BackendHandler::RenderImGui();
		std::cout << "frame";
	}
	
	return 0; 
} 
