//全ての敵のマネージャークラス
#include "EnemyManager.h"
#include "PlayerWeapon.h"
#include <random>
#include "ImGuiManager.h"
#include "SpellManager.h"
#include "Slime.h"
#include "Fly.h"

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
	PlayerWeapon* weapon = PlayerWeapon::GetInstance();
	
	for (int i = 0; i < enemysList_.size(); i++) {
		if (enemysList_[i]->GetIsDead()) {
			enemysList_.erase(enemysList_.begin() + i);
		}
	}

	for (int i = 0; i < enemysList_.size(); i++) {
		enemysList_[i]->Update();

		//プレイヤーが攻撃中なら
		if (weapon->GetIsAt()) {
			//当たり判定検索
			if ((enemysList_[i]->GetPos() - weapon->GetHitPos()).length() < weapon->ATTACK_RADIUS) {
				//当たった判定を敵に与える
				if (SpellManager::GetInstance()->GetActiveEnchantFire()) {
					enemysList_[i]->SetIsHit(EnchantFire::DAMAGE);
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

void EnemyManager::PopEnemy(Vector3 pos)
{
	BaseEnemy* newEnemy = new Enemy;
	newEnemy->Initialize("player",pos);
	enemysList_.push_back(newEnemy);
}

void EnemyManager::PopSlime(Vector3 pos)
{
	BaseEnemy* newEnemy = new Slime;
	newEnemy->Initialize("slime", pos);
	enemysList_.push_back(newEnemy);
}

void EnemyManager::PopFly(Vector3 pos)
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
