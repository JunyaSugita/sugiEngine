#include "EnemyManager.h"

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
	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->SetIsDead();
	}
}

void EnemyManager::Update()
{
	//Á‚·ƒtƒ‰ƒO‚Ì—§‚Á‚½“G‚Ìíœ
	enemys_.remove_if([](unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDead();
	});

	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
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
