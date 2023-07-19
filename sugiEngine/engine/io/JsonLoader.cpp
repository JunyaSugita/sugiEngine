#include "JsonLoader.h"
#include <fstream>

using namespace std;

nlohmann::json JsonLoader::sDeserialized;
LevelData* JsonLoader::sLevelData;

LevelData* JsonLoader::LoadJson(const std::string& filename)
{
	//連結してフルパスを得る
	const string fullpath = "Resources/level/" + filename + ".Json";

	//ファイルストリーム
	ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//解凍
	file >> sDeserialized;

	//正しいレベルデータファイルかチェック
	assert(sDeserialized.is_object());
	assert(sDeserialized.contains("name"));
	assert(sDeserialized["name"].is_string());

	//"name"を文字列として取得
	string name = sDeserialized["name"].get<string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	sLevelData = new LevelData();

	LoadRecursive();

	return sLevelData;
}

void JsonLoader::LoadRecursive()
{
	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : sDeserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//MESH
		if (type.compare("MESH") == 0) {
			//要素追加
			sLevelData->obj.emplace_back(LevelData::OBJData{});
			//今追加した要素の参照を得る
			LevelData::OBJData& objectData = sLevelData->obj.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.filename = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.pos.x = (float)transform["translation"][1];
			objectData.pos.y = (float)transform["translation"][2];
			objectData.pos.z = -(float)transform["translation"][0];
			//回転角
			objectData.rot.x = -(float)transform["rotation"][1];
			objectData.rot.y = -(float)transform["rotation"][2];
			objectData.rot.z = (float)transform["rotation"][0];
			//スケーリング
			objectData.scale.x = (float)transform["scaling"][1];
			objectData.scale.y = (float)transform["scaling"][2];
			objectData.scale.z = (float)transform["scaling"][0];
		}

		//オブジェクト走査の再帰関数
		if (object.contains("children")) {
			LoadRecursive();
		}
	}
}
