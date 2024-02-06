#include "EnchantFire.h"
#include "SpellManager.h"

void EnchantFire::Initialize(const Vector3& pos, const Vector3& vec)
{
	pos;
	vec;
	time_ = TIME_ALIVE;
	spellType_ = BUFF;
	damage_ = 0;
	debuffType_ = D_NONE;
}

void EnchantFire::Update()
{
	SpellManager::GetInstance()->SetEnchantFire();

	if (--time_ <= 0) {
		Dead();
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
