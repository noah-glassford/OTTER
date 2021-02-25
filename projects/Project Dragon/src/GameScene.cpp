#include "GameScene.h"
#include <IBehaviour.h>
#include <CameraControlBehaviour.h>
#include <PhysicsBody.h>
#include <GreyscaleEffect.h>
#include <ColorCorrection.h>
#include <InstantiatingSystem.h>
#include <AssetLoader.h>
#include <Player.h>
#include <WorldBuilderV2.h>
#include "Enemy.h"
#include <Bloom.h>
#include <LightSource.h>
#include <MorphAnimator.h>


void MainGameScene::InitGameScene()
{
	GameScene::RegisterComponentType<Camera>();
	GameScene::RegisterComponentType<RendererComponent>();
	GameScene::RegisterComponentType<BehaviourBinding>();
	GameScene::RegisterComponentType<PhysicsBody>();

	scene = GameScene::Create("GameScene");
	Application::Instance().ActiveScene = scene;
	RenderingManager::activeScene = scene;

	Texture2D::sptr grass = Texture2D::LoadFromFile("image/grass.jpg");
	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
	Texture2D::sptr EarthEnemy = Texture2D::LoadFromFile("image/earthenemytexture.png");
	Texture2D::sptr Barrel = Texture2D::LoadFromFile("image/BARREL.png");
	Texture2D::sptr nine = Texture2D::LoadFromFile("image/9.png");
	//TextureCubeMap::sptr environmentMap = TextureCubeMap::LoadFromImages("image/skybox/ToonSky.jpg");

	Texture2D::sptr hand = Texture2D::LoadFromFile("image/handtexture.png");

	// Creating an empty texture
	Texture2DDescription desc = Texture2DDescription();
	desc.Width = 1;
	desc.Height = 1;
	desc.Format = InternalFormat::RGB8;
	Texture2D::sptr texture2 = Texture2D::Create(desc);
	// Clear it with a white colour
	texture2->Clear();

	ShaderMaterial::sptr Floor_Mat = ShaderMaterial::Create();
	Floor_Mat->Shader = RenderingManager::NoOutline;
	Floor_Mat->Set("s_Diffuse", grass);
	Floor_Mat->Set("s_Specular", noSpec);
	Floor_Mat->Set("u_Shininess", 2.0f);
	Floor_Mat->Set("u_TextureMix", 0.0f);

	ShaderMaterial::sptr handMat = ShaderMaterial::Create();
	handMat->Shader = RenderingManager::BaseShader;
	handMat->Set("s_Diffuse", hand);
	handMat->Set("s_Specular", noSpec);
	handMat->Set("u_Shininess", 1.0f);
	handMat->Set("u_TextureMix", 0.0f);

	ShaderMaterial::sptr EE_MAT = ShaderMaterial::Create();
	EE_MAT->Shader = RenderingManager::NoOutline;
	EE_MAT->Set("s_Diffuse", EarthEnemy);
	EE_MAT->Set("s_Specular", noSpec);
	EE_MAT->Set("u_Shininess", 3.0f);
	EE_MAT->Set("u_TextureMix", 0.0f);


	ShaderMaterial::sptr BarrelMat = ShaderMaterial::Create();
	BarrelMat->Shader = RenderingManager::NoOutline;
	BarrelMat->Set("s_Diffuse", Barrel);
	BarrelMat->Set("s_Specular", noSpec);
	BarrelMat->Set("u_Shininess", 3.0f);
	BarrelMat->Set("u_TextureMix", 0.0f);

	ShaderMaterial::sptr NumMat = ShaderMaterial::Create();
	NumMat->Shader = RenderingManager::NoOutline;
	NumMat->Set("s_Diffuse", nine);
	NumMat->Set("s_Specular", noSpec);
	NumMat->Set("u_Shininess", 3.0f);
	NumMat->Set("u_TextureMix", 0.0f);

	ShaderMaterial::sptr Elm_Cube = ShaderMaterial::Create();
	Elm_Cube->Shader = RenderingManager::NoOutline;
	Elm_Cube->Set("s_Diffuse", noSpec);
	Elm_Cube->Set("s_Specular", noSpec);
	Elm_Cube->Set("u_Shininess", 3.0f);
	Elm_Cube->Set("u_TextureMix", 0.0f);



	// Create an object to be our camera
	GameObject cameraObject = scene->CreateEntity("Camera");
	{
		cameraObject.get<Transform>().SetLocalPosition(0, 0, 0).LookAt(glm::vec3(0, 1, 5));
		cameraObject.emplace<LightSource>();
		//cameraObject.get<Transform>().setForward(glm::vec3(0, 0, -1));
		// We'll make our camera a component of the camera object
		Camera& camera = cameraObject.emplace<Camera>();// Camera::Create();
		Player& player = cameraObject.emplace<Player>();
		player.InitWeapons();
		camera.SetPosition(glm::vec3(0, 3, 3));
		camera.SetUp(glm::vec3(0, 0, 1));
		camera.LookAt(glm::vec3(0));
		camera.SetFovDegrees(90.0f); // Set an initial FOV
		camera.SetOrthoHeight(3.0f);
		PhysicsBody& p = cameraObject.emplace<PhysicsBody>();
		p.AddBody(100.f, btVector3(15, 15, 5), btVector3(1, 1, 2), 3.f);
		p.GetBody()->setCollisionFlags(p.GetBody()->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

		BehaviourBinding::Bind<CameraControlBehaviour>(cameraObject);
	}
	
	GameObject RightHand = scene->CreateEntity("RHand");
	{
		RightHand.get<Transform>().SetLocalPosition(1, -1, 0).SetLocalRotation(-90, 0, 0);
		RightHand.get<Transform>().SetParent(cameraObject);
	
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/hand.obj");
		
		RightHand.emplace<RendererComponent>().SetMesh(vao).SetMaterial(handMat);
	}
	GameObject LeftHand = scene->CreateEntity("LHand");
	{
		LeftHand.get<Transform>().SetLocalPosition(-1, -1, 0).SetLocalRotation(-90, 0, 0).SetLocalScale(-1,1,1);
		LeftHand.get<Transform>().SetParent(cameraObject);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/hand.obj");
		LeftHand.emplace<RendererComponent>().SetMesh(vao).SetMaterial(handMat);
	}

	//test cubes
	GameObject FireCubeVisual = scene->CreateEntity("FireCube");
	{
		FireCubeVisual.get<Transform>().SetParent(RightHand);
		FireCubeVisual.get<Transform>().SetLocalPosition(0, 3, 0).SetLocalRotation(0, 0, 0).SetLocalScale(0.5, 0.5, 0.5);

		//FireCubeVisual.get<Transform>().SetParent(RightHand);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/cube.obj", glm::vec4(1,0,0,1));
		FireCubeVisual.emplace<RendererComponent>().SetMesh(vao).SetMaterial(BarrelMat);
	}

	GameObject WaterCubevisual = scene->CreateEntity("WaterCube");
	{
		WaterCubevisual.get<Transform>().SetParent(RightHand);
		WaterCubevisual.get<Transform>().SetLocalPosition(0, 3, 0).SetLocalRotation(0, 0, 0).SetLocalScale(0.5, 0.5, 0.5);
		WaterCubevisual.emplace<RendererComponent>() = AssetLoader::GetRendererFromStr("Water_Proj");
	}
	GameObject AirCubeVisual = scene->CreateEntity("AirCube");
	{
		AirCubeVisual.get<Transform>().SetParent(LeftHand);
		AirCubeVisual.get<Transform>().SetLocalPosition(0, 3, 0).SetLocalRotation(0, 0, 0).SetLocalScale(0.5, 0.5, 0.5);

		//FireCubeVisual.get<Transform>().SetParent(RightHand);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/cube.obj", glm::vec4(1, 0, 0, 1));
		AirCubeVisual.emplace<RendererComponent>().SetMesh(vao).SetMaterial(EE_MAT);
	}

	GameObject EarthCubeVisual = scene->CreateEntity("EarthCube");
	{
		EarthCubeVisual.get<Transform>().SetParent(LeftHand);
		EarthCubeVisual.get<Transform>().SetLocalPosition(0, 3, 0).SetLocalRotation(0, 0, 0).SetLocalScale(0.5, 0.5, 0.5);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/cube.obj", glm::vec4(0, 0, 1, 1));
		EarthCubeVisual.emplace<RendererComponent>().SetMesh(vao).SetMaterial(Floor_Mat);
		
	}

	GameObject obj3 = scene->CreateEntity("Test Enemy");
	{
		obj3.get<Transform>().SetLocalRotation(90, 0, 0);

		RendererComponent& RC = obj3.emplace<RendererComponent>() = AssetLoader::GetRendererFromStr("Fire_Enemy");
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/cube.obj");



		PhysicsBody& p = obj3.emplace<PhysicsBody>();
		Enemy& e = obj3.emplace<Enemy>();
		p.AddBody(1.f, btVector3(0.f, 0.f, 3.f), btVector3(2.f, 2.f, 2.f));
		p.GetBody()->setUserIndex(2);
		p.GetBody()->setUserPointer((void*)&e);

	}
	/*
	//skybox
	{
		ShaderMaterial::sptr skyboxMat = ShaderMaterial::Create();
		skyboxMat->Shader = RenderingManager::SkyBox;
		skyboxMat->Set("s_Environment", environmentMap);
		skyboxMat->Set("u_EnvironmentRotation", glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))));
		skyboxMat->RenderLayer = 100;
		MeshBuilder<VertexPosNormTexCol> mesh;
		MeshFactory::AddIcoSphere(mesh, glm::vec3(0.0f), 1.0f);
		MeshFactory::InvertFaces(mesh);
		VertexArrayObject::sptr meshVao = mesh.Bake();

		GameObject skyboxObj = scene->CreateEntity("skybox");
		skyboxObj.get<Transform>().SetLocalPosition(0.0f, 0.0f, 0.0f);
		skyboxObj.get_or_emplace<RendererComponent>().SetMesh(meshVao).SetMaterial(skyboxMat);
	}
	*/
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

	WorldBuilderV2 builder;
	builder.BuildNewWorld();

	
}