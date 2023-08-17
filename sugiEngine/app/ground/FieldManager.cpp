#include "FieldManager.h"
#include "EnemyManager.h"
#include "NavePointManager.h"
#include "ModelManager.h"

using namespace std;

FieldManager* FieldManager::GetInstance()
{
	static FieldManager instance;

	return &instance;
}

void FieldManager::Initialize()
{
	// レベルデータの読み込み
	levelData_ = JsonLoader::LoadJson("level");

	objNum_ = 0;
	navePointNum_ = 0;
	for (auto& objectData : levelData_->obj) {
		if (objectData.filename == "box") {
			//モデルを指定して3Dオブジェクトを生成
			obj_[objNum_].Initialize("ground");

			//obj情報
			obj_[objNum_].pos = objectData.pos;
			obj_[objNum_].rot = { 0,0,0 };
			obj_[objNum_].scale = objectData.scale;
			obj_[objNum_].obj->SetColor({ 1,1,1,1 });
			obj_[objNum_].obj->SetIsSimple();
			float tilY = 0;
			if (objectData.scale.x > objectData.scale.y) {
				tilY = objectData.scale.x;
			}
			else {
				
				tilY = objectData.scale.z;
			}

			obj_[objNum_].obj->SetTiling({ objectData.scale.y,tilY });

			BoxCol temp;
			temp.pos = objectData.pos;
			temp.size = objectData.scale;
			col_.push_back(temp);

			objNum_++;
		}
		if (objectData.filename == "ground") {
			//モデルを指定して3Dオブジェクトを生成
			obj_[objNum_].Initialize("ground");

			//obj情報
			obj_[objNum_].pos = objectData.pos;
			obj_[objNum_].rot = { 0,0,0 };
			obj_[objNum_].scale = objectData.scale;
			obj_[objNum_].obj->SetColor({ 1,1,1,1 });
			obj_[objNum_].obj->SetIsSimple();
			obj_[objNum_].obj->SetTiling({ 50,50 });

			BoxCol temp;
			temp.pos = objectData.pos;
			temp.size = objectData.scale;
			col_.push_back(temp);

			objNum_++;
		}
		if (objectData.filename == "enemy") {
			EnemyManager::GetInstance()->PopEnemy(objectData.pos);
		}
		if (objectData.filename == "slime") {
			EnemyManager::GetInstance()->PopSlime(objectData.pos);
		}
		if (objectData.filename == "navePoint") {
			NavePointManager::GetInstance()->Add(objectData.pos);
			navePointNum_++;
		}
	}
}

void FieldManager::GameInitialize()
{
	objNum_ = 0;
	navePointNum_ = 0;
	for (auto& objectData : levelData_->obj) {
		if (objectData.filename == "box") {
			//モデルを指定して3Dオブジェクトを生成
			obj_[objNum_].Initialize("ground");

			//obj情報
			obj_[objNum_].pos = objectData.pos;
			obj_[objNum_].rot = { 0,0,0 };
			obj_[objNum_].scale = objectData.scale;
			obj_[objNum_].obj->SetColor({ 1,1,1,1 });
			obj_[objNum_].obj->SetIsSimple();

			BoxCol temp;
			temp.pos = objectData.pos;
			temp.size = objectData.scale;
			col_.push_back(temp);

			objNum_++;
		}
		if (objectData.filename == "ground") {
			//モデルを指定して3Dオブジェクトを生成
			obj_[objNum_].Initialize("ground");

			//obj情報
			obj_[objNum_].pos = objectData.pos;
			obj_[objNum_].rot = { 0,0,0 };
			obj_[objNum_].scale = objectData.scale;
			obj_[objNum_].obj->SetColor({ 1,1,1,1 });
			obj_[objNum_].obj->SetIsSimple();

			BoxCol temp;
			temp.pos = objectData.pos;
			temp.size = objectData.scale;
			col_.push_back(temp);

			objNum_++;
		}
		if (objectData.filename == "enemy") {
			EnemyManager::GetInstance()->PopEnemy(objectData.pos);
		}
		if (objectData.filename == "slime") {
			EnemyManager::GetInstance()->PopSlime(objectData.pos);
		}
		if (objectData.filename == "navePoint") {
			NavePointManager::GetInstance()->Add(objectData.pos);
			navePointNum_++;
		}
	}
}

void FieldManager::Update()
{
	for (int i = 0; i < objNum_; i++) {
		obj_[i].Update();
	}
}

void FieldManager::Draw()
{
	for (int i = 0; i < objNum_; i++) {
		obj_[i].Draw();
	}
}
