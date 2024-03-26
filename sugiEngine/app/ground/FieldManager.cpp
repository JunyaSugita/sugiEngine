#include "FieldManager.h"
#include "EnemyManager.h"
#include "NaviPointManager.h"
#include "ModelManager.h"
#include "ClearChecker.h"
#include "StageSelectManager.h"
#include "Particle.h"
#include "ColliderManager.h"

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
	NaviPointManager::GetInstance()->Initialize();
	ClearChecker::GetInstance()->ResetHp();

	// レベルデータの読み込み
	LoadStage(num);

	navePointNum_ = 0;
	walls_.clear();
	torchs_.clear();
	objs_.clear();
	for (auto& objectData : levelData_->obj) {
		if (objectData.filename == "box") {
			SetWall(objectData.pos, objectData.scale);
		}
		else if (objectData.filename == "ground") {
			SetFloor(objectData.pos, objectData.scale);
		}
		else if (objectData.filename == "enemy") {
			EnemyManager::GetInstance()->PopEnemy(objectData.pos);
		}
		else if (objectData.filename == "slime") {
			EnemyManager::GetInstance()->PopSlime(objectData.pos);
		}
		else if (objectData.filename == "fly") {
			EnemyManager::GetInstance()->PopFly(objectData.pos);
		}
		else if (objectData.filename == "navePoint") {
			NaviPointManager::GetInstance()->Add(objectData.pos);
			navePointNum_++;
		}
		else if (objectData.filename == "goal") {
			SetGoal(objectData.pos);
		}
		else if (objectData.filename == "torch") {
			bool debug = false;

#ifdef _DEBUG
			debug = true;
#endif // _DEBUG
			if (!debug) {
				SetTorch(objectData.pos, objectData.rot, objectData.scale);
			}
		}
	}
}

void FieldManager::Update()
{
	for (std::unique_ptr<BaseObj>& obj : objs_) {
		obj->Update();
	}
	for (std::unique_ptr<Wall>& w : walls_) {
		w->Update();
	}
	for (std::unique_ptr<Torch>& t : torchs_) {
		t->Update();
	}
}

void FieldManager::Draw()
{
	for (std::unique_ptr<BaseObj>& obj : objs_) {
		obj->Draw();
	}
	for (std::unique_ptr<Wall>& w : walls_) {
		w->Draw();
	}
	for (std::unique_ptr<Torch>& t : torchs_) {
		t->Draw();
	}
}

void FieldManager::LoadStage(int num)
{
	switch (num)
	{
	case TUTORIAL:
		levelData_ = JsonLoader::LoadLevelJson("levelTutorial");
		stageAtten_ = ATTEN_LOW;
		break;
	case STAGE1:
		levelData_ = JsonLoader::LoadLevelJson("level0");
		stageAtten_ = ATTEN_LOW;
		break;
	case STAGE2:
		levelData_ = JsonLoader::LoadLevelJson("level1");
		stageAtten_ = ATTEN_HIGT;
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

void FieldManager::SetWall(const Vector3& pos, const Vector3& scale)
{
	std::unique_ptr<Wall> wall = std::make_unique<Wall>();
	wall->Initialize(pos, scale);
	walls_.push_back(std::move(wall));
}

void FieldManager::SetFloor(const Vector3& pos, const Vector3& scale)
{
	//モデルを指定して3Dオブジェクトを生成
	std::unique_ptr <BaseObj> tempObj = std::make_unique<BaseObj>();
	tempObj->Initialize("ground");

	//obj情報
	tempObj->pos = pos;
	tempObj->rot = { 0,0,0 };
	tempObj->scale = scale;
	tempObj->obj->SetColor({ 1,1,1,1 });
	tempObj->obj->SetTiling({ 100,100 });

	objs_.push_back(std::move(tempObj));

	Col temp;
	temp.pos = pos;
	temp.size = scale;
}

void FieldManager::SetGoal(const Vector3& pos)
{
	ClearChecker::GetInstance()->SetGoal(pos);
	useLightNum_ = lightGroup_->SetPointLightGetNum();
	lightGroup_->SetPointLightColor(useLightNum_, COLOR_GOAL);
	lightGroup_->SetPointLightAtten(useLightNum_, { ATTEN_GOAL,ATTEN_GOAL,ATTEN_GOAL });
	lightGroup_->SetPointLightPos(useLightNum_, { pos.x, pos.y + GOAL_Y,pos.z });
}

void FieldManager::SetTorch(const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
	std::unique_ptr<Torch> torch = std::make_unique<Torch>();
	torch->Initialize(pos, rot, scale, stageAtten_);
	torchs_.push_back(std::move(torch));
}