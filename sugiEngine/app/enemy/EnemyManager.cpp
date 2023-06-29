#include "EnemyManager.h"
#include "PlayerWeapon.h"
#include <random>

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

	//Á‚·ƒtƒ‰ƒO‚Ì—§‚Á‚½“G‚Ìíœ
	enemys_.remove_if([](unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDead();
	});

	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
		//ƒvƒŒƒCƒ„[‚ªUŒ‚’†‚È‚ç
		if (weapon->GetIsAt()) {
			//“–‚½‚è”»’èŒŸõ
			if ((enemy->GetPos() - weapon->GetHitPos()).length() < weapon->ATTACK_LENGTH) {
				//“–‚½‚Á‚½”»’è‚ğ“G‚É—^‚¦‚é
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
	//ƒ‰ƒ“ƒ_ƒ€
	random_device seed_gen;
	mt19937_64 engine(seed_gen());
	uniform_real_distribution<float> rand(0.0f, 5.9f);

	unique_ptr<Enemy> newEnemy = make_unique<Enemy>();
	switch (int(rand(engine)))
	{
	case 1:
		newEnemy->Initialize({40,0,40});
		break;
	case 2:
		newEnemy->Initialize({ -40,0,40 });
		break;
	case 3:
		newEnemy->Initialize({ 40,0,-40 });
		break;
	case 4:
		newEnemy->Initialize({ -40,0,-40 });
		break;

	default:
		newEnemy->Initialize();
		break;
	}

	enemys_.push_back(move(newEnemy));
}

void EnemyManager::ResetIsHit()
{
	for (unique_ptr<Enemy>& enemy : enemys_) {
		enemy->ResetIsHit();
	}
}
