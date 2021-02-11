#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Scene.h>
#include <Transform.h>
static class InstantiatingSystem
{
public:


	//to load a prefab from a file
	static void LoadPrefabFromFile(glm::vec3 origin, std::string filename);
	static std::vector<GameObject> m_Instantiated;
private:
};