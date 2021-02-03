#pragma once
#include <Shader.h>
#include <Scene.h>

static class RenderingManager
{
public:
	static void Init();
	static void Render();
	static Shader::sptr BaseShader;
	static Shader::sptr SkyBox;
	static GameScene::sptr activeScene; //keeps track of the active scene
};