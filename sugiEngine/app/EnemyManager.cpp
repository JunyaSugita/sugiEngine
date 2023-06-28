#include "EnemyManager.h"
#include "PlayerWeapon.h"

using namespace std;

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
}

void EnemyManager::Update()
{
	PlayerWeapon* weapon = PlayerWeapon::GetInstance();

	//消すフラグの立った敵の削除
	enemys_.remove_if([](unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDead();
	});

	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
		//プレイヤーが攻撃中なら
		if (weapon->GetIsAt()) {
			//当たり判定検索
			if ((enemy->GetPos() - weapon->GetHitPos()).length() < weapon->ATTACK_LENGTH) {
				//当たった判定を敵に与える
				enemy->SetIsHit();
			}

		}
	}
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
	newEnemy->Initialize();

	enemys_.push_back(move(newEnemy));
}

void EnemyManager::ResetIsHit()
{
	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->ResetIsHit();
	}
}
