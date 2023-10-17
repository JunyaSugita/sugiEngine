#include "FieldManager.h"
#include "EnemyManager.h"
#include "NavePointManager.h"
#include "ModelManager.h"
#include "ClearChecker.h"
#include "StageSelectManager.h"
#include "ParticleManager.h"

using namespace std;

FieldManager* FieldManager::GetInstance()
{
	static FieldManager instance;

	return &instance;
}

void FieldManager::Initialize(int num)
{
	// レベルデータの読み込み
	SelectStage(num);

	objNum_ = 0;
	navePointNum_ = 0;
	torchNum_ = 0;
	col_.clear();
	for (auto& objectData : levelData_->obj) {
		if (objectData.filename == "box") {
			//モデルを指定して3Dオブジェクトを生成
			obj_[objNum_].Initialize("ground");

			//obj情報
			obj_[objNum_].pos = objectData.pos;
			obj_[objNum_].rot = { 0,0,0 };
			obj_[objNum_].scale = objectData.scale;
			obj_[objNum_].obj->SetColor({ 1,1,1,1 });
			//obj_[objNum_].obj->SetIsSimple();
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
			//obj_[objNum_].obj->SetIsSimple();
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
		if (objectData.filename == "goal") {
			ClearChecker::GetInstance()->SetGoal(objectData.pos);
		}
		if (objectData.filename == "torch") {
			//モデルを指定して3Dオブジェクトを生成
			torchObj_[torchNum_].Initialize("torch");

			//obj情報
			torchObj_[torchNum_].pos = objectData.pos;
			torchObj_[torchNum_].rot.x = objectData.rot.x + 180;
			torchObj_[torchNum_].rot.y = objectData.rot.y - 90;
			torchObj_[torchNum_].rot.z = objectData.rot.z + 90;
			torchObj_[torchNum_].scale = objectData.scale;
			torchObj_[torchNum_].obj->SetColor({0.2f,0.2f,0.2f,1});
			torchNum_++;
		}
	}
}

void FieldManager::Update()
{
	for (int i = 0; i < objNum_; i++) {
		obj_[i].Update();
	}
	for (int i = 0; i < torchNum_;i++) {
		torchObj_[i].Update();
		ParticleManager::GetInstance()->AddFromFile(P_TORCH,torchObj_[i].pos);
	}
}

void FieldManager::Draw()
{
	for (int i = 0; i < objNum_; i++) {
		obj_[i].Draw();
	}
	for (int i = 0; i < torchNum_; i++) {
		torchObj_[i].Draw();
	}
}

void FieldManager::SelectStage(int num)
{
	switch (num)
	{
	case TUTORIAL:
		levelData_ = JsonLoader::LoadJson("levelTutorial");
		break;

	case STAGE1:
		levelData_ = JsonLoader::LoadJson("level");
		break;
	case STAGE2:
		levelData_ = JsonLoader::LoadJson("level2");
		break;
	case SET_SPELL_STAGE:
		levelData_ = JsonLoader::LoadJson("def");
		break;
	default:
		break;
	}
}
