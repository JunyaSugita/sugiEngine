/**
 * @file JsonLoader.h
 * @brief jsonファイルからマップ情報を受け取る
 */

#pragma once
#include <string>
#include <vector>
#include "json.hpp"
#include "SugiMath.h"
#include "BaseSpell.h"

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
	//レベルデータ
	static LevelData* LoadLevelJson(const std::string& filename);
	static void LoadRecursive();

	//呪文データ
	static SpellData LoadSpellData(const std::string& filename);

private:
	static nlohmann::json sDeserialized;
	//レベルデータ
	static LevelData* sLevelData;
};

