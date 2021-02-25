#include "MainMenu.h"
#include <Transform.h>
#include <Camera.h>
#include <RenderingManager.h>
#include <Texture2D.h>
#include <ObjLoader.h>
#include <PhysicsBody.h>
#include <Player.h>
#include <PostEffect.h>
#include <ColorCorrection.h>
#include <Bloom.h>
#include <BackendHandler.h>
#include <IBehaviour.h>
#include <CameraControlBehaviour.h>
void MainMenuScene::InitGameScene()
{

	GameScene::RegisterComponentType<Camera>();
	GameScene::RegisterComponentType<RendererComponent>();
	

	scene = GameScene::Create("GameScene");
	RenderingManager::activeScene = scene;

	Texture2D::sptr menu = Texture2D::LoadFromFile("image/mainmenu.png");
	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
	//Material for menu
	ShaderMaterial::sptr Menu = ShaderMaterial::Create();
	Menu->Shader = RenderingManager::UIShader;
	Menu->Set("s_Diffuse", menu);
	Menu->Set("s_Specular", menu);
	Menu->Set("u_Shininess", 2.0f);
	Menu->Set("u_TextureMix", 0.0f);

	GameObject cameraObject = scene->CreateEntity("Camera");
	{
		cameraObject.get<Transform>().SetLocalPosition(0, 0, 0).LookAt(glm::vec3(0, 1, 5));
		cameraObject.emplace<Player>();
		cameraObject.get<Player>().InitWeapons();
		PhysicsBody& p =cameraObject.emplace<PhysicsBody>();
		p.AddBody(0.f, btVector3(1, 1, 1), btVector3(1, 1, 1));
		//cameraObject.get<Transform>().setForward(glm::vec3(0, 0, -1));
		// We'll make our camera a component of the camera object
		Camera& camera = cameraObject.emplace<Camera>();// Camera::Create();
		
		camera.SetPosition(glm::vec3(0, 3, 3));
		camera.SetUp(glm::vec3(0, 0, 1));
		camera.LookAt(glm::vec3(0));
		camera.SetFovDegrees(90.0f); // Set an initial FOV
		camera.SetOrthoHeight(3.0f);

		BehaviourBinding::Bind<CameraControlBehaviour>(cameraObject);
	}
	GameObject UIObject = scene->CreateEntity("Menu");
	{ 
		RendererComponent& r = UIObject.emplace<RendererComponent>();
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/plane.obj");
		r.SetMaterial(Menu).SetMesh(vao);
	}
	/*
	//if these aren't here it crashes lmao
	BloomEffect* bloom;
	GameObject BloomEffectObject = scene->CreateEntity("Bloom Effect");
	{
		int width, height;
		glfwGetWindowSize(BackendHandler::window, &width, &height);
		bloom = &BloomEffectObject.emplace<BloomEffect>();
		bloom->Init(width, height);
		bloom->SetThreshold(0.1f);
	}
	PostEffect* basicEffect;
	GameObject framebufferObject = scene->CreateEntity("Basic Effect");
	{
		int width, height;
		glfwGetWindowSize(BackendHandler::window, &width, &height);

		basicEffect = &framebufferObject.emplace<PostEffect>();
		basicEffect->Init(width, height);
	}
	//color grading effect
	ColorCorrectionEffect* colorEffect;
	GameObject colorEffectObject = scene->CreateEntity("ColorGrading Effect");
	{
		int width, height;
		glfwGetWindowSize(BackendHandler::window, &width, &height);

		colorEffect = &colorEffectObject.emplace<ColorCorrectionEffect>();
		colorEffect->Init(width, height);

		//number here doesn't matter
		colorEffect->LoadLUT("cube/Neutral-512.cube", 0);
		//colorEffect->LoadLUT("cube/BrightenedCorrectionwarm.cube", 0);
		//colorEffect->LoadLUT("cube/colourcorrectcool.cube", 0);
		//colorEffect->LoadLUT("cube/test.cube",0);
		colorEffect->_LUT = colorEffect->_LUTS[0];
	}
	*/

}
