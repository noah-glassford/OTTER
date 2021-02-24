//
//
//  --  World Builder V2  --
//
//

#include "WorldBuilderV2.h"
#include "PhysicsBody.h"
#include <RendererComponent.h>
#include <AssetLoader.h>
#include <Enemy.h>
#include <LightSource.h>
void WorldBuilderV2::BuildNewWorld()
{
	DestroyCurrentWorld();
	ResetWorldData();
	FillWorldData();

	int temp = 0;
	for each (int i in WorldData) {
		if (i > 0)
			temp += 1;
	}
	std::cout << "\n\n\nWorld Size: " << temp << "\n\n\n";
	GenerateTiles();
}

void WorldBuilderV2::ResetWorldData()
{
	for each (bool i in WorldData) {
		i = 0;
		std::cout << i << '\n';
	}
}

void WorldBuilderV2::DestroyCurrentWorld()
{
	for each (GameObject node in currentWorldGOs) {
		std::cout << "Destroying OBJ In: CurrentWorldGos.";
		RenderingManager::activeScene->RemoveEntity(node);
	}
	currentWorldGOs.clear();
}

void WorldBuilderV2::FillWorldData()
{
	srand(time(NULL));

	bool isBuilding = true;
	bool canLeft, canRight, canUp, canDown;

	int currentX = 15, currentY = 15;
	int pastX, pastY;
	int MovementData = 4;

	while (isBuilding)
	{
		canLeft = false; canRight = false; canUp = false; canDown = false;
		//Sets tile at start
		WorldData[currentX][currentY] = 5;
		pastX = currentX; pastY = currentY;

		//Data check
		if (WorldData[currentX + 1][currentY] < 1 && currentX < 24)
			canRight = true;
		if (WorldData[currentX - 1][currentY] < 1 && currentX > 1)
			canLeft = true;
		if (WorldData[currentX][currentY + 1] < 1 && currentY < 24)
			canUp = true;
		if (WorldData[currentX][currentY - 1] < 1 && currentY > 1)
			canDown = true;

		//Exit Check
		if (!canLeft && !canRight && !canUp && !canDown)
			isBuilding = false;
		else {
			bool temp = false;
			while (!temp) {
				switch (rand() % 4) {
				case 0:
					if (canLeft) {
						currentX -= 1;
						MovementData = 3;
						temp = true;
					}
					break;
				case 1:
					if (canRight) {
						currentX += 1;
						MovementData = 2;
						temp = true;
					}
					break;
				case 2:
					if (canUp) {
						currentY += 1;
						MovementData = 0;
						temp = true;
					}
					break;
				case 3:
					if (canDown) {
						currentY -= 1;
						MovementData = 1;
						temp = true;
					}
					break;
				}
			}
		}
	}
}

void WorldBuilderV2::GenerateTiles()
{
	//
	//	TEMP -- Builds the world of empty tiles -- TEMP
	//
	for (int x = 0; x < 25; x++) {
		for (int y = 0; y < 25; y++) {
			if (WorldData[x][y] > 0) {
				//Floor
				InstantiatingSystem::LoadPrefabFromFile(glm::vec3(x * 20, y * 20, 0)
					, "node/Blank_Floor_Tile.node");
				//places the camera properly
				btTransform t;
				t.setIdentity();
				t.setOrigin(btVector3(x * 20.f, y * 20.f, 2.f));
				RenderingManager::activeScene->FindFirst("Camera").get<PhysicsBody>().GetBody()->setWorldTransform(t);

				currentWorldGOs.push_back(InstantiatingSystem::m_Instantiated[InstantiatingSystem::m_Instantiated.size() - 1]);
				
				//enemy
				if ((x % 3) == 0)
				{
					GameObject FireEn = InstantiatingSystem::InstantiateEmpty("Fire_Enemy");
					FireEn.get<Transform>().SetLocalRotation(90, 0, 0);
					FireEn.emplace<RendererComponent>() = AssetLoader::GetRendererFromStr("Fire Enemy");
					PhysicsBody& p = FireEn.emplace<PhysicsBody>();
					FireEn.get<PhysicsBody>().AddBody(1.f, btVector3(x * 20.f, y * 20.f, 2.f), btVector3(2,2,2));
					Enemy& e = FireEn.emplace<Enemy>();
					p.GetBody()->setUserIndex(2);
					p.GetBody()->setUserPointer((void*)&e);
					FireEn.emplace<LightSource>();
				}
				
				//Roof
				InstantiatingSystem::LoadPrefabFromFile(glm::vec3(x * 20, y * 20, 20)
					, "node/Blank_Floor_Tile.node");

				//
				// Exterior Walls
				//

				if (WorldData[x][y + 1] < 1) {
					InstantiatingSystem::LoadPrefabFromFile(glm::vec3(x * 20, (y * 20) + 10, 10)
						, "node/Blank_Wall_Y.node");
					currentWorldGOs.push_back(InstantiatingSystem::m_Instantiated[InstantiatingSystem::m_Instantiated.size() - 1]);
				}

				if (WorldData[x][y - 1] < 1) {
					InstantiatingSystem::LoadPrefabFromFile(glm::vec3(x * 20, (y * 20) - 10, 10)
						, "node/Blank_Wall_Y.node"); \
						currentWorldGOs.push_back(InstantiatingSystem::m_Instantiated[InstantiatingSystem::m_Instantiated.size() - 1]);
				}

				if (WorldData[x + 1][y] < 1) {
					InstantiatingSystem::LoadPrefabFromFile(glm::vec3((x * 20) + 10, y * 20, 10)
						, "node/Blank_Wall_X.node");
					currentWorldGOs.push_back(InstantiatingSystem::m_Instantiated[InstantiatingSystem::m_Instantiated.size() - 1]);
				}

				if (WorldData[x - 1][y] < 1) {
					InstantiatingSystem::LoadPrefabFromFile(glm::vec3((x * 20) - 10, y * 20, 10)
						, "node/Blank_Wall_X.node");
					currentWorldGOs.push_back(InstantiatingSystem::m_Instantiated[InstantiatingSystem::m_Instantiated.size() - 1]);
				}

				std::cout << "\n[" << x << "][" << y << "]\n";
			}
		}
	}
}