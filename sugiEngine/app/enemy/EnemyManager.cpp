#include "EnemyManager.h"
#include "PlayerWeapon.h"
#include <random>
#include "ImGuiManager.h"
#include "SpellManager.h"
#include "Slime.h"

using namespace std;
using namespace ImGui;

EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;

	return &instance;
}

void EnemyManager::Initialize()
{
	for (unique_ptr<BaseEnemy>& enemy : enemys_) {
		enemy->SetIsDead();
	}
	enemyCount_ = 0;
}

void EnemyManager::GameInitialize()
{
	for (unique_ptr<BaseEnemy>& enemy : enemys_) {
		enemy->SetIsDead();
	}
	//消すフラグの立った敵の削除
	enemys_.remove_if([](unique_ptr<BaseEnemy>& enemy) {
		return enemy->GetIsDead();
	});

	enemyCount_ = 0;
}

void EnemyManager::Update()
{
	PlayerWeapon* weapon = PlayerWeapon::GetInstance();

	//消すフラグの立った敵の削除
	enemys_.remove_if([](unique_ptr<BaseEnemy>& enemy) {
		return enemy->GetIsDead();
	});

	enemyCount_ = 0;
	for (unique_ptr<BaseEnemy>& enemy : enemys_) {
		enemy->Update();

		enemyCount_ += enemy->GetLife();
		//プレイヤーが攻撃中なら
		if (weapon->GetIsAt()) {
			//当たり判定検索
			if ((enemy->GetPos() - weapon->GetHitPos()).length() < weapon->ATTACK_RADIUS) {
				//当たった判定を敵に与える
				if (SpellManager::GetInstance()->GetActiveEnchantFire()) {
					enemy->SetIsHit(10);
					enemy->SetDebuff(D_FIRE,3);
				}
				else {
					enemy->SetIsHit(5,true);
				}
			}
		}
	}
}

void EnemyManager::Draw()
{
	for (unique_ptr<BaseEnemy>& enemy : enemys_) {
		enemy->Draw();
	}
}

void EnemyManager::PopEnemy(Vector3 pos)
{
	unique_ptr<BaseEnemy> newEnemy = make_unique<Enemy>();
	newEnemy->Initialize("player",pos);
	enemyCount_ += newEnemy->GetLife();
	enemys_.push_back(move(newEnemy));
}

void EnemyManager::PopSlime(Vector3 pos)
{
	unique_ptr<BaseEnemy> newEnemy = make_unique<Slime>();
	newEnemy->Initialize("slime", pos);
	enemyCount_ += newEnemy->GetLife();
	enemys_.push_back(move(newEnemy));
}

void EnemyManager::ResetIsHit()
{
	for (unique_ptr<BaseEnemy>& enemy : enemys_) {
		enemy->ResetIsHit();
	}
}

std::vector<BaseEnemy*> EnemyManager::GetEnemysList()
{
	enemysList_.clear();
	for (std::unique_ptr<BaseEnemy>& enemy : enemys_) {
		enemysList_.push_back(enemy.get());
	}
	return enemysList_;
}
