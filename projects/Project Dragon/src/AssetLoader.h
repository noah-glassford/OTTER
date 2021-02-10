#pragma once
#include <vector>
#include <RendererComponent.h>
#include <string>
#include <ShaderMaterial.h>
static class AssetLoader
{
public:
	static void Init();

	static RendererComponent& GetRendererFromStr(std::string name);
	static std::vector<std::string> RendererNames;
	static std::vector<RendererComponent> Renderers;



};