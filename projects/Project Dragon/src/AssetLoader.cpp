#include <AssetLoader.h>
#include <ObjLoader.h>
#include <RenderingManager.h>
#include <Texture2D.h>
#include <MorphAnimator.h>
std::vector<std::string> AssetLoader::RendererNames;
std::vector<RendererComponent> AssetLoader::Renderers;
std::vector<std::string> AssetLoader::GameObjectNames;
std::vector<GameObject> AssetLoader::GameObjects;
void AssetLoader::Init() //doing it manually because, actually I don't know why
{
	//basic floor tile
<<<<<<< Updated upstream
	VertexArrayObject::sptr plane = ObjLoader::LoadFromFile("model/plane.obj");
=======
	VertexArrayObject::sptr plane = ObjLoader::LoadFromFile("model/plane.obj", glm::vec4(1, 1, 1, 1));
	VertexArrayObject::sptr wall = ObjLoader::LoadFromFile("model/wall.obj", glm::vec4(1, 1, 1, 1));
>>>>>>> Stashed changes
	Texture2D::sptr floor = Texture2D::LoadFromFile("image/floor.png");
	Texture2D::sptr test = Texture2D::LoadFromFile("image/test.png"); 
	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
	ShaderMaterial::sptr Floor_Mat = ShaderMaterial::Create();
	Floor_Mat->Shader = RenderingManager::NoOutline;
	Floor_Mat->Set("s_Diffuse", floor);
	Floor_Mat->Set("s_Specular", noSpec);
	Floor_Mat->Set("u_Shininess", 2.0f);
	Floor_Mat->Set("u_TextureMix", 0.0f);
	RendererComponent FloorTile;
	FloorTile.SetMaterial(Floor_Mat).SetMesh(plane);
	Renderers.push_back(FloorTile);
	RendererNames.push_back("Floor");

	ShaderMaterial::sptr Wall_Mat = ShaderMaterial::Create();
	Wall_Mat->Shader = RenderingManager::NoOutline;
	Wall_Mat->Set("s_Diffuse", test);
	Wall_Mat->Set("s_Specular", noSpec);
	Wall_Mat->Set("u_Shininess", 2.0f);
	Wall_Mat->Set("u_TextureMix", 0.0f);
	RendererComponent Wall_Tile;
	Wall_Tile.SetMaterial(Wall_Mat).SetMesh(plane);
	Renderers.push_back(Wall_Tile);
	RendererNames.push_back("Wall");	

	//Water Proj
	VertexArrayObject::sptr Ice_Mesh = ObjLoader::LoadFromFile("model/Ice projectile.obj");
	Texture2D::sptr Ice_Tex = Texture2D::LoadFromFile("image/icetexture.png");
	//	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
	ShaderMaterial::sptr Ice_Mat = ShaderMaterial::Create();
	Ice_Mat->Shader = RenderingManager::BaseShader;
	Ice_Mat->Set("s_Diffuse", Ice_Tex);
	Ice_Mat->Set("s_Specular", noSpec);
	Ice_Mat->Set("u_Shininess", 2.0f);
	Ice_Mat->Set("u_TextureMix", 0.0f);
	RendererComponent Ice_Proj;
	Ice_Proj.SetMaterial(Ice_Mat).SetMesh(Ice_Mesh);
	Renderers.push_back(Ice_Proj);
	RendererNames.push_back("Water_Proj");

	VertexArrayObject::sptr Barrel_Mesh = ObjLoader::LoadFromFile("model/barrel.obj");
	Texture2D::sptr Barrel_Tex = Texture2D::LoadFromFile("image/BARREL.png");
	//	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
	ShaderMaterial::sptr Barrel_Mat = ShaderMaterial::Create();
	Barrel_Mat->Shader = RenderingManager::BaseShader;
	Barrel_Mat->Set("s_Diffuse", Barrel_Tex);
	Barrel_Mat->Set("s_Specular", noSpec);
	Barrel_Mat->Set("u_Shininess", 2.0f);
	Barrel_Mat->Set("u_TextureMix", 0.0f);
	RendererComponent Barrel;
	Barrel.SetMaterial(Barrel_Mat).SetMesh(Barrel_Mesh);
	Renderers.push_back(Barrel);
	RendererNames.push_back("Barrel");
	//box
	{
		VertexArrayObject::sptr box_Mesh = ObjLoader::LoadFromFile("model/box.obj");
		Texture2D::sptr box_tex = Texture2D::LoadFromFile("image/BARREL.png");
		//	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		ShaderMaterial::sptr box_Mat = ShaderMaterial::Create();
		box_Mat->Shader = RenderingManager::BaseShader;
		box_Mat->Set("s_Diffuse", box_tex);
		box_Mat->Set("s_Specular", noSpec);
		box_Mat->Set("u_Shininess", 2.0f);
		box_Mat->Set("u_TextureMix", 0.0f);
		RendererComponent Box;
		Box.SetMaterial(box_Mat).SetMesh(box_Mesh);
		Renderers.push_back(Box);
		RendererNames.push_back("Box");
	}
	//Torch
	{
		VertexArrayObject::sptr box_Mesh = ObjLoader::LoadFromFile("model/torch.obj");
		Texture2D::sptr box_tex = Texture2D::LoadFromFile("image/TORCH.png");
		//	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		ShaderMaterial::sptr box_Mat = ShaderMaterial::Create();
		box_Mat->Shader = RenderingManager::BaseShader;
		box_Mat->Set("s_Diffuse", box_tex);
		box_Mat->Set("s_Specular", noSpec);
		box_Mat->Set("u_Shininess", 2.0f);
		box_Mat->Set("u_TextureMix", 0.0f);
		RendererComponent Box;
		Box.SetMaterial(box_Mat).SetMesh(box_Mesh);
		Renderers.push_back(Box);
		RendererNames.push_back("Torch");
	}
	//Pillar
	{
		VertexArrayObject::sptr box_Mesh = ObjLoader::LoadFromFile("model/pillar.obj");
		Texture2D::sptr box_tex = Texture2D::LoadFromFile("image/stone_001_diffuse.png");
		//	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		ShaderMaterial::sptr box_Mat = ShaderMaterial::Create();
		box_Mat->Shader = RenderingManager::BaseShader;
		box_Mat->Set("s_Diffuse", box_tex);
		box_Mat->Set("s_Specular", noSpec);
		box_Mat->Set("u_Shininess", 2.0f);
		box_Mat->Set("u_TextureMix", 0.0f);
		RendererComponent Box;
		Box.SetMaterial(box_Mat).SetMesh(box_Mesh);
		Renderers.push_back(Box);
		RendererNames.push_back("Pillar");
	}
	//pillar broken
	{
		VertexArrayObject::sptr box_Mesh = ObjLoader::LoadFromFile("model/pillar broken.obj");
		Texture2D::sptr box_tex = Texture2D::LoadFromFile("image/stone_001_diffuse.png");
		//	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		ShaderMaterial::sptr box_Mat = ShaderMaterial::Create();
		box_Mat->Shader = RenderingManager::BaseShader;
		box_Mat->Set("s_Diffuse", box_tex);
		box_Mat->Set("s_Specular", noSpec);
		box_Mat->Set("u_Shininess", 2.0f);
		box_Mat->Set("u_TextureMix", 0.0f);
		RendererComponent Box;
		Box.SetMaterial(box_Mat).SetMesh(box_Mesh);
		Renderers.push_back(Box);
		RendererNames.push_back("Broken_Pillar");
	}
	//Fire Enemy
	{
		VertexArrayObject::sptr box_Mesh = ObjLoader::LoadFromFile("model/FE_I_1.obj");
		Texture2D::sptr box_tex = Texture2D::LoadFromFile("image/FE_TEXTURE.png");
		//	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		ShaderMaterial::sptr box_Mat = ShaderMaterial::Create();
		box_Mat->Shader = RenderingManager::BaseShader;
		box_Mat->Set("s_Diffuse", box_tex);
		box_Mat->Set("s_Specular", noSpec);
		box_Mat->Set("u_Shininess", 2.0f);
		box_Mat->Set("u_TextureMix", 0.0f);
		RendererComponent Box;
		Box.SetMaterial(box_Mat).SetMesh(box_Mesh);
		Renderers.push_back(Box);
		RendererNames.push_back("Fire_Enemy");
	}
	//Water Enemy
	{
		VertexArrayObject::sptr box_Mesh = ObjLoader::LoadFromFile("model/waterenemy.obj");
		Texture2D::sptr box_tex = Texture2D::LoadFromFile("image/stone_001_diffuse.png");
		Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		ShaderMaterial::sptr box_Mat = ShaderMaterial::Create();
		box_Mat->Shader = RenderingManager::BaseShader;
		box_Mat->Set("s_Diffuse", box_tex);
		box_Mat->Set("s_Specular", noSpec);
		box_Mat->Set("u_Shininess", 2.0f);
		box_Mat->Set("u_TextureMix", 0.0f);
		RendererComponent Box;
		Box.SetMaterial(box_Mat).SetMesh(box_Mesh);
		Renderers.push_back(Box);
		RendererNames.push_back("Water_Enemy");
	}
	//Earth Enemy
	{
		VertexArrayObject::sptr box_Mesh = ObjLoader::LoadFromFile("model/Earth_En.obj");
		Texture2D::sptr box_tex = Texture2D::LoadFromFile("image/earthenemytexture.png");
		//	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		ShaderMaterial::sptr box_Mat = ShaderMaterial::Create();
		box_Mat->Shader = RenderingManager::BaseShader;
		box_Mat->Set("s_Diffuse", box_tex);
		box_Mat->Set("s_Specular", noSpec);
		box_Mat->Set("u_Shininess", 2.0f);
		box_Mat->Set("u_TextureMix", 0.0f);
		RendererComponent Box;
		Box.SetMaterial(box_Mat).SetMesh(box_Mesh);
		Renderers.push_back(Box);
		RendererNames.push_back("Earth_Enemy");
	}
	//Air Enemy
	{
		VertexArrayObject::sptr box_Mesh = ObjLoader::LoadFromFile("model/air_enemy.obj");
		Texture2D::sptr box_tex = Texture2D::LoadFromFile("image/stone_001_diffuse.png");
		//	Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		ShaderMaterial::sptr box_Mat = ShaderMaterial::Create();
		box_Mat->Shader = RenderingManager::BaseShader;
		box_Mat->Set("s_Diffuse", box_tex);
		box_Mat->Set("s_Specular", noSpec);
		box_Mat->Set("u_Shininess", 2.0f);
		box_Mat->Set("u_TextureMix", 0.0f);
		RendererComponent Box;
		Box.SetMaterial(box_Mat).SetMesh(box_Mesh);
		Renderers.push_back(Box);
		RendererNames.push_back("Air_Enemy");
	}
<<<<<<< Updated upstream
=======

	//model for player hands
	{
		Texture2D::sptr hand = Texture2D::LoadFromFile("image/handtexture.png");
		ShaderMaterial::sptr handMat = ShaderMaterial::Create();
		handMat->Shader = RenderingManager::BaseShader;
		handMat->Set("s_Diffuse", hand);
		handMat->Set("s_Specular", noSpec);
		handMat->Set("u_Shininess", 1.0f);
		handMat->Set("u_TextureMix", 0.0f);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/hand_rest.obj");
		RendererComponent hands;
		hands.SetMesh(vao).SetMaterial(handMat);
		Renderers.push_back(hands);
		RendererNames.push_back("hands");
	}

	{
		Texture2D::sptr hpBar = Texture2D::LoadFromFile("image/HP_FULL.png");
		//Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		//Material for menu
		ShaderMaterial::sptr HP = ShaderMaterial::Create();
		HP->Shader = RenderingManager::UIShader;
		HP->Set("s_Diffuse", hpBar);
		HP->Set("s_Specular", hpBar);
		HP->Set("u_Shininess", 2.0f);
		HP->Set("u_TextureMix", 0.0f);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/plane.obj");
		RendererComponent hp;
		hp.SetMaterial(HP).SetMesh(vao);
		Renderers.push_back(hp);
		RendererNames.push_back("hpBar");
	}
	//crosshairs
	{
		Texture2D::sptr hpBar = Texture2D::LoadFromFile("image/crosshair.png");
		//Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		//Material for menu
		ShaderMaterial::sptr HP = ShaderMaterial::Create();
		HP->Shader = RenderingManager::UIShader;
		HP->Set("s_Diffuse", hpBar);
		HP->Set("s_Specular", hpBar);
		HP->Set("u_Shininess", 2.0f);
		HP->Set("u_TextureMix", 0.0f);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/plane.obj");
		RendererComponent hp;
		hp.SetMaterial(HP).SetMesh(vao);
		Renderers.push_back(hp);
		RendererNames.push_back("crosshair");
	}

	{
		Texture2D::sptr menu = Texture2D::LoadFromFile("image/deathscreen.png");

		//Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		//Material for menu
		ShaderMaterial::sptr HP = ShaderMaterial::Create();
		HP->Shader = RenderingManager::UIShader;
		HP->Set("s_Diffuse", menu);
		HP->Set("s_Specular", menu);
		HP->Set("u_Shininess", 2.0f);
		HP->Set("u_TextureMix", 0.0f);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/plane.obj");
		RendererComponent hp;
		hp.SetMaterial(HP).SetMesh(vao);
		Renderers.push_back(hp);
		RendererNames.push_back("DeathScreen");
	}

	{
		Texture2D::sptr menu = Texture2D::LoadFromFile("image/MainMenu.png");
		Texture2D::sptr noSpec = Texture2D::LoadFromFile("image/grassSpec.png");
		//Material for menu
		ShaderMaterial::sptr Menu = ShaderMaterial::Create();
		Menu->Shader = RenderingManager::UIShader;
		Menu->Set("s_Diffuse", menu);
		Menu->Set("s_Specular", menu);
		Menu->Set("u_Shininess", 2.0f);
		Menu->Set("u_TextureMix", 0.0f);
		VertexArrayObject::sptr vao = ObjLoader::LoadFromFile("model/plane.obj");
		RendererComponent hp;
		hp.SetMaterial(Menu).SetMesh(vao);
		Renderers.push_back(hp);
		RendererNames.push_back("MainMenu");
	}
>>>>>>> Stashed changes
}
RendererComponent& AssetLoader::GetRendererFromStr(std::string name)
{
	for (int i = 0; i < Renderers.size(); i++)
	{
		if (name == RendererNames[i])
		{
			return Renderers[i];
		}
	}
	return Renderers[0]; //backup
}
GameObject& AssetLoader::GetGameObjectFromStr(std::string name)
{
	for (int i = 0; i < GameObjectNames.size(); i++)
	{
		if (name == GameObjectNames[i])
		{
			return GameObjects[i];
		}
	}
	return GameObjects[0]; //backup
}
/*
Mesh& AssetLoader::GetMeshFromStr(std::string name)
{
	// TODO: insert return statement here
	for (int i = 0; i < m_Meshes.size(); i++)
	{
		if (name == m_MeshNames[i])
		{
			return m_Meshes[i];
		}
	}
	return m_Meshes[0];

}

Material& AssetLoader::GetMatFromStr(std::string name)
{
	// TODO: insert return statement here

	for (int i = 0; i < m_Materials.size(); i++)
	{
		if (name == m_MaterialNames[i])
		{
			return m_Materials[i];
		}
	}
	return m_Materials[0];
}
*/