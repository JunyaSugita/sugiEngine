#include "ClearChecker.h"
#include "EnemyManager.h"
#include "GameManager.h"
#include "PostEffectSecond.h"
#include "UIManager.h"
#include "ParticleManager.h"

ClearChecker* ClearChecker::GetInstance()
{
	static ClearChecker instance;

	return &instance;
}

void ClearChecker::Initialize()
{
	obj_.Initialize("goal");
	obj_.obj->SetColor({ 0,0,1,0.5f });

	col_.Initialize();
	col_.col.size = { 1,3,1 };

	isClear_ = false;
	blur_ = 0;
}

void ClearChecker::GameInitialize()
{
	isClear_ = false;
	blur_ = 0;
}

void ClearChecker::Update()
{
	if (isClear_ == true) {
		if (blur_ < 5) {
			blur_ += 0.025f;
			if (blur_ >= 1) {
				PostEffectSecond::SetBlur((int32_t)blur_);
			}
		}
		else {
			UIManager::GetInstance()->SetClear();
		}
	}

	obj_.Update();
	col_.SetCol(obj_.pos);
	col_.Update();

	ParticleManager::GetInstance()->AddFromFile(P_GOAL, { obj_.pos.x, obj_.pos.y + 3 ,obj_.pos.z });
}

void ClearChecker::Draw()
{
	obj_.Draw();
	col_.Draw();
}

void ClearChecker::SetGoal(Vector3 pos)
{
	obj_.pos = pos;
	col_.col.pos = pos;
	isClear_ = false;
	blur_ = 0;
}
