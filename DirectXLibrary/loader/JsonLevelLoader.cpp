#include "JsonLevelLoader.h"

#include <assert.h>

const std::string JSON_LEVEL_PATH = "Resources/Level/";

gamelib::LevelData::ObjectData gamelib::JsonLevelLoader::ReadSceneObject(std::ifstream& inFile)
{
	std::string line_data;
	LevelData::ObjectData object;

	std::getline(inFile, line_data, ',');
	size_t index = line_data.rfind(" ") + 2;
	object.name = line_data.substr(index, line_data.size() - (index + 1));

	//"translation"
	std::getline(inFile, line_data, '[');
	assert(line_data.find("translation") != std::string::npos);
	inFile >> line_data;
	object.translation.x = std::stof(line_data);
	inFile >> line_data;
	object.translation.z = std::stof(line_data);
	inFile >> line_data;
	object.translation.y = std::stof(line_data);

	//"rotation"
	std::getline(inFile, line_data, '[');
	assert(line_data.find("rotation") != std::string::npos);
	inFile >> line_data;
	object.rotation.x = std::stof(line_data);
	inFile >> line_data;
	object.rotation.z = std::stof(line_data);
	inFile >> line_data;
	object.rotation.y = std::stof(line_data);

	//"scaling"
	std::getline(inFile, line_data, '[');
	assert(line_data.find("scaling") != std::string::npos);
	inFile >> line_data;
	object.scaling.x = std::stof(line_data);
	inFile >> line_data;
	object.scaling.z = std::stof(line_data);
	inFile >> line_data;
	object.scaling.y = std::stof(line_data);
	std::getline(inFile, line_data, ',');

	//出力
	printf("name : %s\n", object.name.c_str());
	printf("translation : (%f, %f, %f)\n", object.translation.x, object.translation.y, object.translation.z);
	printf("rotation : (%f, %f, %f)\n", object.rotation.x, object.rotation.y, object.rotation.z);
	printf("scaling : (%f, %f, %f)\n", object.scaling.x, object.scaling.y, object.scaling.z);

	//"file_name"
	inFile >> line_data;
	if (line_data.find("file_name") != std::string::npos)
	{
		inFile >> line_data;
		object.fileName = line_data.substr(1, line_data.size() - 2);
		printf("file_name : %s\n", object.fileName.c_str());
		inFile >> line_data;
	}

	//"collider"
	if (line_data.find("collider") != std::string::npos)
	{
		std::getline(inFile, line_data, ',');
		size_t index = line_data.rfind(" ") + 2;
		std::string collider = line_data.substr(index, line_data.size() - (index + 1));

		std::getline(inFile, line_data, '[');
		assert(line_data.find("center") != std::string::npos);
		Vector3 center;
		inFile >> line_data;
		center.x = std::stof(line_data);
		inFile >> line_data;
		center.y = std::stof(line_data);
		inFile >> line_data;
		center.z = std::stof(line_data);
		object.boxColliderParam.center = center;

		std::getline(inFile, line_data, '[');
		assert(line_data.find("size") != std::string::npos);
		Vector3 size;
		inFile >> line_data;
		size.x = std::stof(line_data);
		inFile >> line_data;
		size.y = std::stof(line_data);
		inFile >> line_data;
		size.z = std::stof(line_data);
		object.boxColliderParam.size = size;
		std::getline(inFile, line_data, ',');

		printf("collider type : %s\n", collider.c_str());
		printf("center : (%f, %f, %f)\n", center.x, center.y, center.z);
		printf("size : (%f, %f, %f)\n", size.x, size.y, size.z);
	}
	return object;
}

gamelib::LevelData* gamelib::JsonLevelLoader::LoadFile(const std::string& fileName)
{
	const std::string fullpath = JSON_LEVEL_PATH + fileName + ".json";
	std::ifstream inFile;
	inFile.open(fullpath);
	//ファイルチェック
	assert(!inFile.fail() && "ファイルが見つかりませんでした");

	std::string line;
	//一番初めの波括弧
	inFile >> line;

	//"name"
	inFile >> line;
	assert(line.find("name") != std::string::npos);

	inFile >> line;
	std::string name = line.substr(1, line.size() - 3);
	assert(name.compare("scene") == 0);

	std::getline(inFile, line, '{');
	assert(line.find("objects") != std::string::npos);
	LevelData* levelData = new LevelData();

	while (std::getline(inFile, line, ','))
	{
		if (line.find("MESH") != std::string::npos)
		{
			LevelData::ObjectData object(ReadSceneObject(inFile));
			levelData->objects.emplace_back(object);
			printf("---------------------------------------\n");
		}
	}
	inFile.close();
	return levelData;
}

