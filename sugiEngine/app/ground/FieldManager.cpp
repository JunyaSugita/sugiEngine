#include "FieldManager.h"
#include "EnemyManager.h"
#include "NavePointManager.h"
#include "ModelManager.h"
#include "ClearChecker.h"
#include "StageSelectManager.h"
#include "ParticleManager.h"

using namespace std;

LightGroup* FieldManager::lightGroup_ = nullptr;

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
			useLightNum_ = lightGroup_->SetPointLightGetNum();
			lightGroup_->SetPointLightColor(useLightNum_, { 0,0,1 });
			lightGroup_->SetPointLightAtten(useLightNum_, { 0.002f,0.002f,0.002f });
			lightGroup_->SetPointLightPos(useLightNum_, { objectData.pos.x, objectData.pos.y + 2,objectData.pos.z });
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
			
			useLightNum_ = lightGroup_->SetPointLightGetNum();
			lightGroup_->SetPointLightColor(useLightNum_,{1,0.2f,0});
			lightGroup_->SetPointLightAtten(useLightNum_,{ stageAtten_,stageAtten_,stageAtten_ });
			lightGroup_->SetPointLightPos(useLightNum_, { torchObj_[torchNum_].pos.x, torchObj_[torchNum_].pos.y ,torchObj_[torchNum_].pos.z });

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
		levelData_ = JsonLoader::LoadJson("levelTutorial2");
		stageAtten_ = 0.01f;
		break;
	case STAGE1:
		levelData_ = JsonLoader::LoadJson("levelTutorial");
		stageAtten_ = 0.01f;
		break;
	case STAGE2:
		levelData_ = JsonLoader::LoadJson("level");
		stageAtten_ = 0.001f;
		break;
	case SET_SPELL_STAGE:
		levelData_ = JsonLoader::LoadJson("def");
		stageAtten_ = 0.01f;
		break;
	default:
		break;
	}
}
