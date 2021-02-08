
#include "GameScene.h"
#include <IBehaviour.h>
#include <CameraControlBehaviour.h>
#include <PhysicsBody.h>
#include <GreyscaleEffect.h>
#include <ColorCorrection.h>

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
	Texture2D::sptr FireEnemy = Texture2D::LoadFromFile("image/FE_TEXTURE.png");
	TextureCubeMap::sptr environmentMap = TextureCubeMap::LoadFromImages("image/skybox/ToonSky.jpg");

	Texture2D::sptr hand = Texture2D::LoadFromFile("image/handtexture.png");


	// Creating an empty texture
	Texture2DDescription desc = Texture2DDescription();
	desc.Width = 1;
	desc.Height = 1;
	desc.Format = InternalFormat::RGB8;
	Texture2D::sptr texture2 = Texture2D::Create(desc);
	// Clear it with a white colour
	texture2->Clear();
	
	ShaderMaterial::sptr grassMat = ShaderMaterial::Create();
	grassMat->Shader = RenderingManager::BaseShader;
	grassMat->Set("s_Diffuse", grass);
	grassMat->Set("s_Specular", noSpec);
	grassMat->Set("u_Shininess", 2.0f);
	grassMat->Set("u_TextureMix", 0.0f);

	ShaderMaterial::sptr handMat = ShaderMaterial::Create();
	handMat->Shader = RenderingManager::BaseShader;
	handMat->Set("s_Diffuse", hand);
	handMat->Set("s_Specular", noSpec);
	handMat->Set("u_Shininess", 1.0f);
	handMat->Set("u_TextureMix", 0.0f);

	ShaderMaterial::sptr FE_Mat = ShaderMaterial::Create();
	FE_Mat->Shader = RenderingManager::BaseShader;
	FE_Mat->Set("s_Diffuse", FireEnemy);
	FE_Mat->Set("s_Specular", noSpec);
	FE_Mat->Set("u_Shininess", 3.0f);
	FE_Mat->Set("u_TextureMix", 0.0f);

	GameObject obj1 = scene->CreateEntity("Ground");
	{
		obj1.get<Transform>().SetLocalPosition(0, 0, 0);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/plane.obj");
		obj1.emplace<RendererComponent>().SetMesh(vao).SetMaterial(grassMat);
		obj1.emplace<PhysicsBody>();
		obj1.get<PhysicsBody>().AddBody(0.f, btVector3(0, 0, 0), btVector3(30, 30, 1));
	}
	GameObject obj2 = scene->CreateEntity("Hand");
	{
		obj2.get<Transform>().SetLocalPosition(0, -5, 3);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/hand.obj");
		obj2.emplace<RendererComponent>().SetMesh(vao).SetMaterial(handMat);
		
	}
	GameObject obj3 = scene->CreateEntity("Enemy");
	{
		obj3.get<Transform>().SetLocalPosition(0, -5, 3);
		obj3.get<Transform>().SetLocalRotation(90, 0, 0);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/Fire_Enemy.obj");
		obj3.emplace<RendererComponent>().SetMesh(vao).SetMaterial(FE_Mat);

	}
	// Create an object to be our camera
	GameObject cameraObject = scene->CreateEntity("Camera");
	{
		cameraObject.get<Transform>().SetLocalPosition(0, 0, 0).LookAt(glm::vec3(0, 0, 0));
		//cameraObject.get<Transform>().setForward(glm::vec3(0, 0, -1));
		// We'll make our camera a component of the camera object
		Camera& camera = cameraObject.emplace<Camera>();// Camera::Create();
		camera.SetPosition(glm::vec3(0, 3, 3));
		camera.SetUp(glm::vec3(0, 0, 1));
		camera.LookAt(glm::vec3(0));
		camera.SetFovDegrees(90.0f); // Set an initial FOV
		camera.SetOrthoHeight(3.0f);

		cameraObject.emplace<PhysicsBody>();
		cameraObject.get<PhysicsBody>().AddBody(1.f, btVector3(0, 0, 5), btVector3(1, 1, 1));
	
		BehaviourBinding::Bind<CameraControlBehaviour>(cameraObject);

	}
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

	//Blank post processing effect
	PostEffect* basicEffect;
	GameObject framebufferObject = scene->CreateEntity("Basic Effect");
	{
		int width, height;
		glfwGetWindowSize(BackendHandler::window, &width, &height);

		basicEffect = &framebufferObject.emplace<PostEffect>();
		basicEffect->Init(width, height);
	}

	//greyscale effect
	GreyscaleEffect* greyscaleEffect;
	GameObject greyScaleEffectObject = scene->CreateEntity("Greyscale Effect");
	{
		int width, height;
		glfwGetWindowSize(BackendHandler::window, &width, &height);
		greyscaleEffect = &greyScaleEffectObject.emplace<GreyscaleEffect>();
		greyscaleEffect->Init(width, height);
		greyscaleEffect->SetIntensity(0.f);
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

	

}