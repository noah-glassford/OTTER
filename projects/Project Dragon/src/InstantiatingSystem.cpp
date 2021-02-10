#include <InstantiatingSystem.h>
#include <AssetLoader.h>
#include <BtToGlm.h>
#include <RenderingManager.h>
// Borrowed from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
#pragma region String Trimming

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

#pragma endregion 



void InstantiatingSystem::LoadPrefabFromFile(glm::vec3 origin, std::string filename)
{

	
	//similar to OBJ loader
	std::stringstream ss;
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file)
		throw std::runtime_error("Failed to open file");

	std::string line = "";
	std::string prefix = "";
	//will only get changed whenever a new entity is created from the file
	std::string GOName;
	while (std::getline(file, line))
	{
		

		
		trim(line);
		ss = std::stringstream(line); 
		ss >> prefix;
		if (prefix == "#")
		{
			//comment nothing happens
		}
		if (prefix == "GO")
		{
			//create a new game object
			
			ss >> GOName;//this will only get updated upon a new game object
			
			GameObject Current = RenderingManager::activeScene->CreateEntity(GOName);
			
		}
		else if (prefix == "RC")
		{
			std::string RendererName;
			ss >> RendererName;
			RenderingManager::activeScene->FindFirst(GOName).emplace<RendererComponent>();
			RenderingManager::activeScene->FindFirst(GOName).get<RendererComponent>() = AssetLoader::GetRendererFromStr(RendererName);
		}
		else if (prefix == "TRNS")
		{
			glm::vec3 pos, scale, rot;
			//load data in order of pos scale rot
			ss >> pos.x >> pos.y >> pos.z >> scale.x >> scale.y >> scale.z >> rot.x >> rot.y >> rot.z;
			
			RenderingManager::activeScene->FindFirst(GOName).get<Transform>().SetLocalPosition(pos + origin).SetLocalScale(scale).SetLocalRotation(rot);

		}
		
		
	
	}

}
