#include "EnemyManager.h"
#include "PlayerWeapon.h"
#include <random>
#include "ImGuiManager.h"
#include "SpellManager.h"

using namespace std;
using namespace ImGui;

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;

	return &instance;
}

void EnemyManager::Initialize()
{
	Enemy::OneTimeInitialize();
	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->SetIsDead();
	}
	enemyCount_ = 0;
}

void EnemyManager::Update()
{
	PlayerWeapon* weapon = PlayerWeapon::GetInstance();

	//消すフラグの立った敵の削除
	enemys_.remove_if([](unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDead();
	});

	enemyCount_ = 0;
	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();

		enemyCount_++;
		//プレイヤーが攻撃中なら
		if (weapon->GetIsAt()) {
			//当たり判定検索
			if ((enemy->GetPos() - weapon->GetHitPos()).length() < weapon->ATTACK_LENGTH) {
				//当たった判定を敵に与える
				enemy->SetIsHit(10,20);
			}
		}
	}

	//Begin("EnemyState");
	//Text("RemainingEnemies %d",enemyCount);
	//End();
}

void EnemyManager::Draw()
{
	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw();
	}
}

void EnemyManager::PopEnemy(Vector3 pos)
{
	unique_ptr<Enemy> newEnemy = make_unique<Enemy>();

	if (pos.y == -1) {
		//ランダム
		random_device seed_gen;
		mt19937_64 engine(seed_gen());
		uniform_real_distribution<float> rand(0.0f, 3.9f);

		switch (int(rand(engine)))
		{
		case 1:
			newEnemy->Initialize({ 40,0,40 });
			break;
		case 2:
			newEnemy->Initialize({ -40,0,40 });
			break;
		case 3:
			newEnemy->Initialize({ 40,0,-40 });
			break;

		default:
			newEnemy->Initialize({ -40,0,-40 });
			break;
		}
	}
	else {
		newEnemy->Initialize(pos);
	}

	enemys_.push_back(move(newEnemy));
}

void EnemyManager::ResetIsHit()
{
	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->ResetIsHit();
	}
}
