#pragma once
#include <string>
#include <vector>

#include "../math/Vector.h"
#include <fstream>

namespace gamelib
{

struct LevelData
{
	struct ObjectData
	{
		std::string name;
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
		struct BoxColliderParam
		{
			Vector3 center;
			Vector3 size;
		};
		BoxColliderParam boxColliderParam;
	};
	std::vector<ObjectData> objects;
};

class JsonLevelLoader
{
private:
	JsonLevelLoader() = delete;
	~JsonLevelLoader() = delete;

	static LevelData::ObjectData ReadSceneObject(std::ifstream& inFile);

public:
	static LevelData* LoadFile(const std::string& fileName);

};
} // namespace gamelib

