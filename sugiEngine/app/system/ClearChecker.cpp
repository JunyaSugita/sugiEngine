#include "ClearChecker.h"
#include "EnemyManager.h"
#include "GameManager.h"
#include "PostEffectSecond.h"
#include "UIManager.h"

void ClearChecker::Initialize()
{
	maxEnemy_ = nowEnemy_ = EnemyManager::GetInstance()->GetEnemyCount();
	gauge_.Set({ WIN_WIDTH / 2,100 }, {800,50},{0.6f,0,0});

	blur_ = 0;
}

void ClearChecker::Update()
{
	nowEnemy_ = EnemyManager::GetInstance()->GetEnemyCount();

	gauge_.Update((float)maxEnemy_, (float)nowEnemy_);


	if (nowEnemy_ <= 0) {
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
}

void ClearChecker::Draw()
{
	gauge_.Draw();
}
