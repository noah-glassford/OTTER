//
//
//  --  World Builder V2  --
//
//

#include "WorldBuilderV2.h"

void WorldBuilderV2::BuildNewWorld()
{
	DestroyCurrentWorld();
	ResetWorldData();
	FillWorldData();
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
		RenderingManager::activeScene->RemoveEntity(node);
	}
	currentWorldGOs.clear();
}

void WorldBuilderV2::FillWorldData()
{
	srand(time(NULL));

	bool isBuilding = true;
	int currentX = 15, currentY = 15;

	bool canLeft, canRight, canUp, canDown;

	while (isBuilding)
	{
		canLeft = false; canRight = false; canUp = false; canDown = false;
		//Sets tile at start
		WorldData[currentX][currentY] = 5;

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
						temp = true;
					}
					break;
				case 1:
					if (canRight) {
						currentX += 1;
						temp = true;
					}
					break;
				case 2:
					if (canUp) {
						currentY += 1;
						temp = true;
					}
					break;
				case 3:
					if (canDown) {
						currentY -= 1;
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
				InstantiatingSystem::LoadPrefabFromFile(glm::vec3(x * 25, y * 25, 0)
					, "node/Blank_Floor_Tile.node");
				std::cout << "\n[" << x << "][" << y << "]\n";
			}
		}
	}
}