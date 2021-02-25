#pragma once
#include <Scene.h>
class SceneBase
{
public:
	virtual void InitGameScene() {};
	GameScene::sptr scene;
};