#pragma once
#include <Shader.h>
#include <Scene.h>
#include <GBuffer.h>
#include <IlluminationBuffer.h>
#include <Bloom.h>
#include <GreyscaleEffect.h>
#include <ColorCorrection.h>
#include "Framebuffer.h"
static class RenderingManager
{
public:
	static void Init();
	static void Render();
	static void InitPostEffects();
	static Shader::sptr BaseShader;
	static Shader::sptr NoOutline;

	static Shader::sptr SkyBox;
	static Shader::sptr Passthrough;
	static Shader::sptr AnimationShader;
	static Shader::sptr UIShader;
	static Shader::sptr simpleDepthShader;
	static Shader::sptr BoneAnimShader;

	static GameScene::sptr activeScene; //keeps track of the active scene

	static Framebuffer* shadowBuf;
	static PostEffect* postEffect;
	static BloomEffect* bloomEffect;
	static ColorCorrectionEffect* colEffect;
	static GBuffer* gBuffer;
	static IlluminationBuffer* illuminationBuffer;
	static Framebuffer* shadowBuffer;

};