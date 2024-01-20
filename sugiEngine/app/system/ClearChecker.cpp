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

	BaseCol::Initialize(obj_.pos,obj_.scale,GOAL);
	col_.size = { 1,3,1 };

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
		if (blur_ < MAX_BLUR) {
			blur_ += SPEED_BLUR;
			if (blur_ >= 1) {
				PostEffectSecond::SetBlur((int32_t)blur_);
			}
		}
		else {
			UIManager::GetInstance()->SetClear();
		}
	}

	obj_.Update();
	BaseCol::Update(obj_.pos,obj_.scale);

	ParticleManager::GetInstance()->AddFromFile(P_GOAL, { obj_.pos.x, obj_.pos.y + GOAL_Y ,obj_.pos.z });
}

void ClearChecker::Draw()
{
	obj_.Draw();
}

void ClearChecker::SetGoal(Vector3 pos)
{
	obj_.pos = pos;
	isClear_ = false;
	blur_ = 0;
}
