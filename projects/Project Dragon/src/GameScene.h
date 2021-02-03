#pragma once
#include "Application.h"
#include <Scene.h>
#include <GameObjectTag.h>
#include <Texture2D.h>
#include <ShaderMaterial.h>
#include "RenderingManager.h"
#include <VertexArrayObject.h>
#include <ObjLoader.h>
#include <RendererComponent.h>
#include "BackendHandler.h"
#include <TextureCubeMap.h>
class MainGameScene
{
public:
	void InitGameScene();
	GameScene::sptr scene;
};