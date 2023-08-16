#include "EnchantFire.h"
#include "SpellManager.h"

void EnchantFire::Initialize(Vector3 pos, Vector3 vec)
{
	time_ = TIME_ALIVE;
	spellType_ = BUFF;
	damage_ = 0;
	debuffType_ = D_NONE;
}

void EnchantFire::Update()
{
	SpellManager::GetInstance()->SetEnchantFire();

	if (--time_ <= 0) {
		isDead_ = true;
	}
}

void EnchantFire::Fire()
{
	time_ = TIME_ALIVE;
}

bool EnchantFire::GetActive()
{
	if (time_) {
		return true;
	}
	return false;
}
