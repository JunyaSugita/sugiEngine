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
	lightGroup_->PointLightAllClear();

	// レベルデータの読み込み
	SelectStage(num);

	objNum_ = 0;
	navePointNum_ = 0;
	torchs_.clear();
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
			std::unique_ptr<Torch> torch = std::make_unique<Torch>();
			torch->Initialize(objectData.pos, objectData.rot, objectData.scale,stageAtten_);
			torchs_.push_back(std::move(torch));
		}
	}
}

void FieldManager::Update()
{
	for (int i = 0; i < objNum_; i++) {
		obj_[i].Update();
	}
	for (std::unique_ptr<Torch>& t : torchs_) {
		t->Update();
	}
}

void FieldManager::Draw()
{
	for (int i = 0; i < objNum_; i++) {
		obj_[i].Draw();
	}
	for (std::unique_ptr<Torch>& t : torchs_) {
		t->Draw();
	}
}

void FieldManager::SelectStage(int num)
{
	switch (num)
	{
	case TUTORIAL:
		levelData_ = JsonLoader::LoadJson("levelTutorial");
		stageAtten_ = 0.01f;
		break;
	case STAGE1:
		levelData_ = JsonLoader::LoadJson("level2");
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

void FieldManager::SetLight(LightGroup* lightGroup)
{
	lightGroup_ = lightGroup;
	Torch::SetLight(lightGroup);
}
