#pragma once
#include <vector>
#include <RendererComponent.h>
#include <string>
#include <ShaderMaterial.h>
#include <Scene.h>
static class AssetLoader
{
public:
	static void Init();

	static RendererComponent& GetRendererFromStr(std::string name);
	static GameObject& GetGameObjectFromStr(std::string name);
	static std::vector<std::string> RendererNames;
	static std::vector<RendererComponent> Renderers;
	static std::vector<GameObject> GameObjects;
	static std::vector<std::string> GameObjectNames;



};