
#include "GameScene.h"
#include <IBehaviour.h>
#include <CameraControlBehaviour.h>
#include <PhysicsBody.h>
#include "Framebuffer.h"
void MainGameScene::InitGameScene()
{
	GameScene::RegisterComponentType<Camera>();
	GameScene::RegisterComponentType<RendererComponent>();
	GameScene::RegisterComponentType<BehaviourBinding>();

	scene = GameScene::Create("GameScene");
	Application::Instance().ActiveScene = scene;
	RenderingManager::activeScene = scene;

	Texture2D::sptr grass = Texture2D::LoadFromFile("image/Stone_001_Diffuse.png");
	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/Stone_001_Specular.png");
	TextureCubeMap::sptr environmentMap = TextureCubeMap::LoadFromImages("image/skybox/ToonSky.jpg");

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

	GameObject obj1 = scene->CreateEntity("Ground");
	{
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/plane.obj");
		obj1.emplace<RendererComponent>().SetMesh(vao).SetMaterial(grassMat);


		//adds physics body
		PhysicsBody& phys = obj1.emplace<PhysicsBody>();
		phys.AddBody(1.f, btVector3(0, 99, 3), btVector3(1, 2, 1));
	}
	// Create an object to be our camera
	GameObject cameraObject = scene->CreateEntity("Camera");
	{
		cameraObject.get<Transform>().SetLocalPosition(0, 3, 3).LookAt(glm::vec3(0, 0, 0));

		// We'll make our camera a component of the camera object
		Camera& camera = cameraObject.emplace<Camera>();// Camera::Create();
		camera.SetPosition(glm::vec3(0, 3, 3));
		camera.SetUp(glm::vec3(0, 0, 1));
		camera.LookAt(glm::vec3(0));
		camera.SetFovDegrees(90.0f); // Set an initial FOV
		camera.SetOrthoHeight(3.0f);
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

	//frame buffer
	Framebuffer* testBuffer;
	GameObject framebufferObject = scene->CreateEntity("Basic Buffer");
	{
		int width, height;
		glfwGetWindowSize(BackendHandler::window, &width, &height);

		testBuffer = &framebufferObject.emplace<Framebuffer>();
		testBuffer->AddDepthTarget();
		testBuffer->AddColorTarget(GL_RGBA8);
		testBuffer->Init(width, height);
	}



}