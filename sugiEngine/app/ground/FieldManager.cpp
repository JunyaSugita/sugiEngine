#include "FieldManager.h"
#include "EnemyManager.h"
#include "NaviPointManager.h"
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
	LoadStage(num);

	navePointNum_ = 0;
	torchs_.clear();
	objs_.clear();
	cols_.clear();
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
			bool isDebug = false;
#ifdef _DEBUG
			isDebug = true;
#endif // _DEBUG
			if (!isDebug) {
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
	for (std::unique_ptr<Torch>& t : torchs_) {
		t->Update();
	}
}

void FieldManager::Draw()
{
	for (std::unique_ptr<BaseObj>& obj : objs_) {
		obj->Draw();
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
		levelData_ = JsonLoader::LoadJson("levelTutorial");
		stageAtten_ = ATTEN_LOW;
		break;
	case STAGE1:
		levelData_ = JsonLoader::LoadJson("level2");
		stageAtten_ = ATTEN_LOW;
		break;
	case STAGE2:
		levelData_ = JsonLoader::LoadJson("level");
		stageAtten_ = ATTEN_HIGT;
		break;
	case SET_SPELL_STAGE:
		levelData_ = JsonLoader::LoadJson("def");
		stageAtten_ = ATTEN_LOW;
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

void FieldManager::SetWall(Vector3 pos, Vector3 scale)
{
	std::unique_ptr <BaseObj> tempObj = std::make_unique<BaseObj>();
	//モデルを指定して3Dオブジェクトを生成
	tempObj->Initialize("ground");

	//obj情報
	tempObj->pos = pos;
	tempObj->rot = { 0,0,0 };
	tempObj->scale = scale;
	tempObj->obj->SetColor({ 1,1,1,1 });
	float tilY = 0;
	if (scale.x > scale.y) {
		tilY = scale.x;
	}
	else {

		tilY = scale.z;
	}

	tempObj->obj->SetTiling({ scale.y,tilY });

	objs_.push_back(std::move(tempObj));

	BoxCol temp;
	temp.pos = pos;
	temp.size = scale;
	cols_.push_back(temp);
}

void FieldManager::SetFloor(Vector3 pos, Vector3 scale)
{
	//モデルを指定して3Dオブジェクトを生成
	std::unique_ptr <BaseObj> tempObj = std::make_unique<BaseObj>();
	tempObj->Initialize("ground");

	//obj情報
	tempObj->pos = pos;
	tempObj->rot = { 0,0,0 };
	tempObj->scale = scale;
	tempObj->obj->SetColor({ 1,1,1,1 });
	tempObj->obj->SetTiling({ 50,50 });

	objs_.push_back(std::move(tempObj));

	BoxCol temp;
	temp.pos = pos;
	temp.size = scale;
	cols_.push_back(temp);
}

void FieldManager::SetGoal(Vector3 pos)
{
	ClearChecker::GetInstance()->SetGoal(pos);
	useLightNum_ = lightGroup_->SetPointLightGetNum();
	lightGroup_->SetPointLightColor(useLightNum_, COLOR_GOAL);
	lightGroup_->SetPointLightAtten(useLightNum_, { ATTEN_GOAL,ATTEN_GOAL,ATTEN_GOAL });
	lightGroup_->SetPointLightPos(useLightNum_, { pos.x, pos.y + GOAL_Y,pos.z });
}

void FieldManager::SetTorch(Vector3 pos, Vector3 rot, Vector3 scale)
{
	std::unique_ptr<Torch> torch = std::make_unique<Torch>();
	torch->Initialize(pos, rot, scale, stageAtten_);
	torchs_.push_back(std::move(torch));
}