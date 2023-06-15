#pragma once
#include <string>
#include <vector>
#include "json.hpp"
#include "SugiMath.h"
#include "FbxModel.h"

struct LevelData {
	struct OBJData {
		std::string filename;

		Vector3 pos;
		Vector3 scale;
		Vector3 rot;
	};

	std::vector<OBJData> obj;
};

class JsonLoader
{
public:
	static LevelData* LoadJson(const std::string& filename);
	static void LoadRecursive();
private:
	static nlohmann::json sDeserialized;
	static LevelData* sLevelData;
};

