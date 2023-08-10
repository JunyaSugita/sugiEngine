#include "EnemyManager.h"
#include "PlayerWeapon.h"
#include <random>
#include "ImGuiManager.h"
#include "SpellManager.h"

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
	//Á‚·ƒtƒ‰ƒO‚Ì—§‚Á‚½“G‚Ìíœ
	enemys_.remove_if([](unique_ptr<BaseEnemy>& enemy) {
		return enemy->GetIsDead();
	});

	enemyCount_ = 0;
}

void EnemyManager::Update()
{
	PlayerWeapon* weapon = PlayerWeapon::GetInstance();

	//Á‚·ƒtƒ‰ƒO‚Ì—§‚Á‚½“G‚Ìíœ
	enemys_.remove_if([](unique_ptr<BaseEnemy>& enemy) {
		return enemy->GetIsDead();
	});

	enemyCount_ = 0;
	for (unique_ptr<BaseEnemy>& enemy : enemys_) {
		enemy->Update();

		enemyCount_ += enemy->GetLife();
		//ƒvƒŒƒCƒ„[‚ªUŒ‚’†‚È‚ç
		if (weapon->GetIsAt()) {
			//“–‚½‚è”»’èŒŸõ
			if ((enemy->GetPos() - weapon->GetHitPos()).length() < weapon->ATTACK_RADIUS) {
				//“–‚½‚Á‚½”»’è‚ğ“G‚É—^‚¦‚é
				if (SpellManager::GetInstance()->GetActiveEnchantFire()) {
					enemy->SetIsHit(10, 20);
					enemy->SetDebuff(FIRE,3);
				}
				else {
					enemy->SetIsHit(5, 20);
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
	newEnemy->Initialize(pos);
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
