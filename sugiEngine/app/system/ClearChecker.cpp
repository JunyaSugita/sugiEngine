#include "ClearChecker.h"
#include "EnemyManager.h"
#include "GameManager.h"

void ClearChecker::Initialize()
{
	maxEnemy_ = nowEnemy_ = EnemyManager::GetInstance()->GetEnemyCount();
	gauge_.Set({ WIN_WIDTH / 2,100 }, {500,50});
}

void ClearChecker::Update()
{
	nowEnemy_ = EnemyManager::GetInstance()->GetEnemyCount();

	gauge_.Update((float)maxEnemy_, (float)nowEnemy_);


	if (nowEnemy_ <= 0) {
		
	}
}

void ClearChecker::Draw()
{
	gauge_.Draw();
}
