#pragma once
#include <vector>
#include <Scene.h>
#include <RenderingManager.h>
#include <InstantiatingSystem.h>
#include <random>
class WorldBuilderV2
{
public:
	//
	// -- Data --
	//

	//Stores the GOs in the world, used mainly in DestroyCurrentWorld()
	std::vector<GameObject> currentWorldGOs;

	//TrueFalse data for the world || helps track if there is already a tile placed on a location
	int WorldData[25][25]{ 0 };

	//	--	 Use this for changine the node size, will update the world get		-- //
	const int nodeSize = 40;

	//
	// Entry and Exit Points:
	//		0 = Top | 1 = Bottom | 2 = Right | 3 = Left | 4 = Null
	//
	int WorldDataEAndEPoints[25][25][1];

	//
	// -- Functions --
	//

	//Builds a new world for the game.
	void BuildNewWorld();
	//Resets the world data, called on by BuildNewWorld()
	void ResetWorldData();
	//Destroys all the GOs stored in currentWorldGOs, called on by BuildNewWorld()
	void DestroyCurrentWorld();
	//Fills the WorldData array
	void FillWorldData();
	//Fills the world reletive to the data collected in FillWorldData()
	void GenerateTiles();
	//	WIP
	//		Populate the world with enemies (fills the currentEnemy variable)
	//  WIP
	void PopulateWorld(int dificulty);
	//Dificualy - 1 is normal, scale off of that
};
