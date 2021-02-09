#include <InstantiatingSystem.h>
#include <AssetLoader.h>
#include <BtToGlm.h>
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



void InstantiatingSystem::LoadPrefabFromFile(int index, glm::vec3 origin, std::string filename)
{

	/*
	//similar to OBJ loader
	std::stringstream ss;
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file)
		throw std::runtime_error("Failed to open file");

	std::string line = "";
	std::string prefix = "";
	//will only get changed whenever a new entity is created from the file
	int ENumb = 0;
	while (std::getline(file, line))
	{
		
		trim(line);
		ss = std::stringstream(line); 
		ss >> prefix;
		if (prefix == "#")
		{
			//comment nothing happens
		}
		if (prefix == "E")
		{
			//create a new entity
			int fileIndex;
			ss >> fileIndex;
			ENumb = index + fileIndex;
			ECS::Create(ENumb);
			std::cout << "Created entity: "<< ENumb << '\n';
		}
		else if (prefix == "STRNS")
		{
			if (!ECS::Has<Transform>(ENumb))
				ECS::Add<Transform>(ENumb);

			glm::vec3 tempPos, tempScale;
			glm::vec4 tempRot;
			ss >> tempPos.x >> tempPos.y >> tempPos.z >> tempScale.x >> tempScale.y >> tempScale.z >> tempRot.x >> tempRot.y >> tempRot.z >> tempRot.w; //Loads all the positions from the file into our vectors

			//Then set the active entity with those positions
			ECS::Get<Transform>(ENumb).SetPosition(tempPos);
			ECS::Get<Transform>(ENumb).SetScale(tempScale);
			ECS::Get<Transform>(ENumb).SetRotation(glm::vec3(tempRot.x, tempRot.y, tempRot.z), tempRot.w); 
		}
		else if (prefix == "SMAT")
		{
			if (!ECS::Has<Material>(ENumb))
				ECS::Add<Material>(ENumb);


			std::string matName;
			ss >> matName;
			ECS::Get<Material>(ENumb) = AssetLoader::GetMatFromStr(matName);
		
		}
		else if (prefix == "SMESH")
		{
			if (!ECS::Has<Mesh>(ENumb))
				ECS::Add<Mesh>(ENumb);

			std::string MeshName;
			ss >> MeshName;
			ECS::Get<Mesh>(ENumb).SetVAO(AssetLoader::GetMeshFromStr(MeshName).GetVAO()); 
		
		}
		else if (prefix == "SPHYS")
		{
			if (!ECS::Has<PhysicsBody>(ENumb))
				ECS::Add<PhysicsBody>(ENumb);

			float mass, friction;
			glm::vec3 position, size;
			ss >> mass >> position.x >> position.y >> position.z >> size.x >> size.y >> size.z >> friction;
			ECS::Get<PhysicsBody>(ENumb).AddBody(mass, BtToGlm::GLMTOBTV3(position), BtToGlm::GLMTOBTV3(size), friction);
		}
	
	}
*/
}
