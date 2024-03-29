#include "ClearChecker.h"
#include "EnemyManager.h"
#include "GameManager.h"
#include "PostEffectSecond.h"
#include "UIManager.h"
#include "ParticleManager.h"
#include "ColliderManager.h"

ClearChecker* ClearChecker::GetInstance()
{
	static ClearChecker instance;

	return &instance;
}

void ClearChecker::Initialize()
{
	obj_.Initialize("goal");
	obj_.obj->SetColor({ 0,0,1,0.5f });

	BaseCol::Initialize(obj_.pos, obj_.scale, GOAL);
	col_.size = { 1,3,1 };

	isClear_ = false;
	blur_ = 0;
	nowHp_ = maxHp_;
	enemyGauge_.Set({ WIN_WIDTH / 2,100 }, { 500,50 }, { 1,0,0 });
}

void ClearChecker::GameInitialize()
{
	isClear_ = false;
	blur_ = 0;
	nowHp_ = maxHp_;
	enemyGauge_.Set({ 100,100 }, { 100,100 }, { 1,0,0 });
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
	BaseCol::Update(obj_.pos, obj_.scale);
	enemyGauge_.Update(maxHp_, nowHp_);

	//if (nowHp_ <= 0) {
	ParticleManager::GetInstance()->AddFromFile(P_GOAL, { obj_.pos.x, obj_.pos.y + GOAL_Y ,obj_.pos.z });
	//}
}

void ClearChecker::Draw()
{
	//if (nowHp_ <= 0) {
	obj_.Draw();
	//}
}

void ClearChecker::SpriteDraw()
{
	enemyGauge_.Draw();
}

void ClearChecker::ResetHp()
{
	maxHp_ = nowHp_ = 0;
}

void ClearChecker::OnCollision(BaseCol* a)
{
	if (a->GetColType() == PLAYER) {
		if (ColliderManager::GetInstance()->CheckHitBox(a->GetCol(), GetCol())) {
			SetClear();
		}
	}
}

void ClearChecker::SetGoal(const Vector3& pos)
{
	obj_.pos = pos;
	isClear_ = false;
	blur_ = 0;
}
