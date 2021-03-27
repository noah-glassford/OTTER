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

#include <UI.h>
#include <DirectionalLight.h>
#include <UniformBuffer.h>
#include <SkinnedMesh.h>

#include <GBuffer.h>
#include <IlluminationBuffer.h>

void MainGameScene::InitGameScene()
{
	GameScene::RegisterComponentType<Camera>();
	GameScene::RegisterComponentType<RendererComponent>();
	GameScene::RegisterComponentType<BehaviourBinding>();
	GameScene::RegisterComponentType<PhysicsBody>();

	scene = GameScene::Create("GameScene");
	Application::Instance().ActiveScene = scene;
	RenderingManager::activeScene = scene;

	
	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
	Texture2D::sptr EarthEnemy = Texture2D::LoadFromFile("image/earthenemytexture.png");
	Texture2D::sptr Barrel = Texture2D::LoadFromFile("image/BARREL.png");
	Texture2D::sptr FloorTexture = Texture2D::LoadFromFile("image/Stone_001_Diffuse.png");
	Texture2D::sptr nine = Texture2D::LoadFromFile("image/9.png");
	TextureCubeMap::sptr environmentMap = TextureCubeMap::LoadFromImages("image/skybox/ToonSky.jpg");

	

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
	Floor_Mat->Set("s_Diffuse", FloorTexture);
	Floor_Mat->Set("s_Specular", noSpec);
	Floor_Mat->Set("u_Shininess", 3.0f);
	Floor_Mat->Set("u_TextureMix", 0.0f);
	

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
		player.m_Hp = 5;
		camera.SetPosition(glm::vec3(0, 3, 3));
		camera.SetUp(glm::vec3(0, 0, 1));
		camera.LookAt(glm::vec3(0));
		camera.SetFovDegrees(90.0f); // Set an initial FOV
		camera.SetOrthoHeight(3.0f);
		PhysicsBody& p = cameraObject.emplace<PhysicsBody>();
		p.AddBody(100.f, btVector3(15, 15, 5), btVector3(1, 1, 2), 3.f);
		p.GetBody()->setCollisionFlags(p.GetBody()->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		p.GetBody()->setUserIndex(1);
		p.GetBody()->setUserIndex3((unsigned)cameraObject.entity());

		BehaviourBinding::Bind<CameraControlBehaviour>(cameraObject);
	}
	
	GameObject UIObject = scene->CreateEntity("PlayerHPBar");
	{
		RendererComponent& r = UIObject.emplace<RendererComponent>();
		
		UI& ui = UIObject.emplace<UI>();
		ui.offset = glm::vec2(0, 0.2);
		ui.scale = glm::vec2(1, 1);
		r = AssetLoader::GetRendererFromStr("hpBar");
	}
	
	/*
	* I can't have nice things this is broken :(
	GameObject Crosshair = scene->CreateEntity("Crosshair");
	{
		RendererComponent& r = Crosshair.emplace<RendererComponent>();

		UI& ui = Crosshair.emplace<UI>();
		ui.offset = glm::vec2(0, -0.2);
		ui.scale = glm::vec2(0.1, 0.1);
		r = AssetLoader::GetRendererFromStr("crosshair");
	}
	
	*/

	//creates directional light gameobject
	GameObject sunObj = scene->CreateEntity("SUN");
	{
		sunObj.emplace<DirectionalLight>();
	}
	GameObject sunBufferObj = scene->CreateEntity("SunBuffer");
	{
		UniformBuffer& sunBuf = sunBufferObj.emplace<UniformBuffer>();
		sunBuf.AllocateMemory(sizeof(DirectionalLight));
		sunBuf.SendData(reinterpret_cast<void*>(&sunObj.get<DirectionalLight>()), sizeof(DirectionalLight));
		sunBuf.Bind(0);
	}


	GameObject RightHand = scene->CreateEntity("RHand");
	{
		RightHand.get<Transform>().SetLocalPosition(1, -0.7, -0.5).SetLocalRotation(-0, 0, 90).SetLocalScale(0.5, 0.5, 0.5);
		RightHand.get<Transform>().SetParent(cameraObject);

		

		RightHand.emplace<RendererComponent>() = AssetLoader::GetRendererFromStr("hands");
	}
	GameObject LeftHand = scene->CreateEntity("LHand");
	{
		LeftHand.get<Transform>().SetLocalPosition(-1, -0.7, -0.5).SetLocalRotation(-0, 0, -90).SetLocalScale(-0.5, 0.5, 0.5);
		LeftHand.get<Transform>().SetParent(cameraObject);
		LeftHand.emplace<RendererComponent>() = AssetLoader::GetRendererFromStr("hands");

	}

	GameObject obj4 = scene->CreateEntity("Barrel");
	{
		/*
		obj4.get<Transform>().SetLocalRotation(0, 0, 0);
		obj4.get<Transform>().SetLocalScale(50, 50, 50);
		obj4.get<Transform>().SetLocalPosition(1000, 1000, 1000);

		obj4.emplace<GLTFSkinnedMesh>();
		obj4.get<GLTFSkinnedMesh>().LoadFromFile("model/JellyCube.gltf");
		obj4.get<GLTFSkinnedMesh>().SetLooping(1);
		*/
	}

	//test cubes
	GameObject FireCubeVisual = scene->CreateEntity("FireCube");
	{
		FireCubeVisual.get<Transform>().SetParent(LeftHand);
		FireCubeVisual.get<Transform>().SetLocalPosition(0, 3, 0).SetLocalRotation(0, 0, 0).SetLocalScale(0.5, 0.5, 0.5);

		//FireCubeVisual.get<Transform>().SetParent(RightHand);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/cube.obj", glm::vec4(1, 0, 0, 1));
		FireCubeVisual.emplace<RendererComponent>().SetMesh(vao).SetMaterial(BarrelMat).CastShadows = false;
	}

	GameObject WaterCubevisual = scene->CreateEntity("WaterCube");
	{
		WaterCubevisual.get<Transform>().SetParent(LeftHand);
		WaterCubevisual.get<Transform>().SetLocalPosition(0, 3, 0).SetLocalRotation(0, 0, 0).SetLocalScale(0.5, 0.5, 0.5);
		WaterCubevisual.emplace<RendererComponent>() = AssetLoader::GetRendererFromStr("Water_Proj");
			WaterCubevisual.get<RendererComponent>().CastShadows = false;
	}
	GameObject AirCubeVisual = scene->CreateEntity("AirCube");
	{
		AirCubeVisual.get<Transform>().SetParent(RightHand);
		AirCubeVisual.get<Transform>().SetLocalPosition(0, 3, 0).SetLocalRotation(0, 0, 0).SetLocalScale(0.5, 0.5, 0.5);

		//FireCubeVisual.get<Transform>().SetParent(RightHand);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/cube.obj", glm::vec4(1, 0, 0, 1));
		AirCubeVisual.emplace<RendererComponent>().SetMesh(vao).SetMaterial(EE_MAT).CastShadows = false;
	}

	GameObject EarthCubeVisual = scene->CreateEntity("EarthCube");
	{
		EarthCubeVisual.get<Transform>().SetParent(RightHand);
		EarthCubeVisual.get<Transform>().SetLocalPosition(0, 3, 0).SetLocalRotation(0, 0, 0).SetLocalScale(0.5, 0.5, 0.5);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/cube.obj", glm::vec4(0, 0, 1, 1));
		EarthCubeVisual.emplace<RendererComponent>().SetMesh(vao).SetMaterial(Floor_Mat).CastShadows = false;
	}

	
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

	GBuffer* gBuffer;
	IlluminationBuffer* illuminationBuffer;


	int width, height;
	glfwGetWindowSize(BackendHandler::window, &width, &height);

	GameObject gBufferObject = scene->CreateEntity("G Buffer");
	{
		gBuffer = &gBufferObject.emplace<GBuffer>();
		gBuffer->Init(width, height);
	}

	GameObject illuminationbufferObject = scene->CreateEntity("Illumination Buffer");
	{
		illuminationBuffer = &illuminationbufferObject.emplace<IlluminationBuffer>();
		illuminationBuffer->Init(width, height);
	}

	Framebuffer* shadowBuffer;
	int shadowWidth = 4096;
	int shadowHeight = 4096;

	GameObject shadowBufferObject = scene->CreateEntity("Shadow Buffer");
	{
		shadowBuffer = &shadowBufferObject.emplace<Framebuffer>();
		shadowBuffer->AddDepthTarget();
		shadowBuffer->Init(shadowWidth, shadowHeight);
	}

	ShaderMaterial::sptr skyboxMat = ShaderMaterial::Create();
	skyboxMat->Shader = RenderingManager::SkyBox;
	skyboxMat->Set("s_Environment", environmentMap);
	skyboxMat->Set("u_EnvironmentRotation", glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))));
	skyboxMat->RenderLayer = 100;

	//skybox
	MeshBuilder<VertexPosNormTexCol> mesh;
	MeshFactory::AddIcoSphere(mesh, glm::vec3(0.0f), 1.0f);
	MeshFactory::InvertFaces(mesh);
	VertexArrayObject::sptr meshVao = mesh.Bake();

	GameObject skyboxObj = scene->CreateEntity("skybox");
	skyboxObj.get<Transform>().SetLocalPosition(500.f, 500.0f, 200.0f);
	skyboxObj.get_or_emplace<RendererComponent>().SetMesh(meshVao).SetMaterial(skyboxMat);


	WorldBuilderV2 builder;
	builder.BuildNewWorld();
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(1000, 1000, 1000));
	//cameraObject.get<PhysicsBody>().GetBody()->setWorldTransform(t);
	//obj4.get<Transform>().SetLocalPosition(cameraObject.get<Transform>().GetLocalPosition());
}