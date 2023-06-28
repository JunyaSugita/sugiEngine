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
