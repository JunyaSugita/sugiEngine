//全ての敵のマネージャークラス
#include "EnemyManager.h"
#include "Player.h"
#include <random>
#include "ImGuiManager.h"
#include "SpellManager.h"
#include "Slime.h"
#include "Fly.h"
#include "ClearChecker.h"
#include "Status.h"

using namespace std;
using namespace ImGui;

EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;

	return &instance;
}

void EnemyManager::Initialize()
{
	enemysList_.clear();
}

void EnemyManager::GameInitialize()
{
	enemysList_.clear();
}

void EnemyManager::Update()
{
	Player* player = Player::GetInstance();
	
	ClearChecker::GetInstance()->ResetNowHp();

	for (int i = 0; i < enemysList_.size(); i++) {
		if (enemysList_[i]->GetIsDead()) {
			enemysList_.erase(enemysList_.begin() + i);
		}
	}

	for (int i = 0; i < enemysList_.size(); i++) {
		enemysList_[i]->Update();

		if (player->GetWeapon() == nullptr) {
			break;
		}
		//プレイヤーが攻撃中なら
		if (player->GetWeapon()->GetIsAt()) {
			//当たり判定検索
			if ((enemysList_[i]->GetPos() - player->GetWeapon()->GetHitPos()).length() < player->GetWeapon()->ATTACK_RADIUS) {
				//当たった判定を敵に与える
				if (SpellManager::GetInstance()->GetActiveEnchantFire()) {
					enemysList_[i]->SetIsHit(Status::GetInstance()->GetSpellData(ENCHANT_FIRE).damage);
					enemysList_[i]->SetDebuff(D_FIRE, EnchantFire::TIME_DEBUFF);
				}
				else {
					enemysList_[i]->SetIsHit(5,true);
				}
			}
		}
	}
}

void EnemyManager::Draw()
{
	for (int i = 0; i < enemysList_.size(); i++) {
		enemysList_[i]->Draw();
	}
}

void EnemyManager::DrawTransparent()
{
	for (int i = 0; i < enemysList_.size(); i++) {
		enemysList_[i]->DrawTransparent();
	}
}

void EnemyManager::PopEnemy(const Vector3& pos)
{
	BaseEnemy* newEnemy = new Enemy;
	newEnemy->Initialize("player",pos);
	enemysList_.push_back(newEnemy);
}

void EnemyManager::PopSlime(const Vector3& pos)
{
	BaseEnemy* newEnemy = new Slime;
	newEnemy->Initialize("slime", pos);
	enemysList_.push_back(newEnemy);
}

void EnemyManager::PopFly(const Vector3& pos)
{
	BaseEnemy* newEnemy = new Fly;
	newEnemy->Initialize("box", pos);
	enemysList_.push_back(newEnemy);
}

void EnemyManager::ResetIsHit()
{
	for (int i = 0; i < enemysList_.size(); i++) {
		enemysList_[i]->ResetIsHit();
	}
}
