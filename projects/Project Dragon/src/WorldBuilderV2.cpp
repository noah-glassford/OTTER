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
	bool canLeft, canRight, canUp, canDown;

	int currentX = 0, currentY = 0;
	int pastX, pastY;
	int MovementData = 4;

	while (isBuilding)
	{
		canLeft = false; canRight = false; canUp = false; canDown = false;
		//Sets tile at start
		WorldData[currentX][currentY] = 5;
		pastX = currentX; pastY = currentY;
		WorldDataEAndEPoints[currentX][currentY][0] = MovementData;

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
			WorldDataEAndEPoints[pastX][pastY][1] = MovementData;
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
				InstantiatingSystem::LoadPrefabFromFile(glm::vec3(x * 20, y * 20, 0)
					, "node/Blank_Floor_Tile.node");

				int top = 0, bottom = 0, right = 0, left = 0;

				switch (WorldDataEAndEPoints[x][y][0]) {
				case 0:
					bottom += 5;
					break;
				case 1:
					top += 5;
					break;
				case 2:
					left += 5;
					break;
				case 3:
					right += 5;
					break;
				case 4:
					break;
				}
				switch (WorldDataEAndEPoints[x][y][1]) {
				case 0:
					top += 5;
					break;
				case 1:
					bottom += 5;
					break;
				case 2:
					right += 5;
					break;
				case 3:
					left += 5;
					break;
				case 4:
					break;
				}

				//Building
				/*
				if (top < 5) {
					InstantiatingSystem::LoadPrefabFromFile(glm::vec3(x * 20, (y * 20) + 10, 0)
						, "node/Blank_Wall_Y.node");
				}
				if (bottom < 5) {
					InstantiatingSystem::LoadPrefabFromFile(glm::vec3(x * 20, (y * 20) - 10, 0)
						, "node/Blank_Wall_Y.node");
				}
				if (right < 5) {
					InstantiatingSystem::LoadPrefabFromFile(glm::vec3((x * 20) + 10, y * 20, 0)
						, "node/Blank_Wall_X.node");
				}
				if (left < 5) {
					InstantiatingSystem::LoadPrefabFromFile(glm::vec3((x * 20) - 10, y * 20, 0)
						, "node/Blank_Wall_X.node");
				}
				*/
				std::cout << "\n[" << x << "][" << y << "]\n";
				std::cout << "E & E Points[" << WorldDataEAndEPoints[x][y][0] << "]["
					<< WorldDataEAndEPoints[x][y][1] << "]\n";
			}
		}
	}
}