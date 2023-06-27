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
	Set();
}

void EnemyManager::Update()
{
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

void EnemyManager::Set(Vector3 pos)
{
	unique_ptr<Enemy> newEnemy = make_unique<Enemy>();
	newEnemy->Initialize();

	enemys_.push_back(move(newEnemy));
}
