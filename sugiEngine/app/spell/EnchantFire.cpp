#include "EnchantFire.h"
#include "SpellManager.h"

void EnchantFire::Initialize(const Vector3& pos, const Vector3& vec)
{
	pos;
	vec;
	spellData_.timeAlive = TIME_ALIVE;
	spellData_.spellType = BUFF;
	spellData_.damage = 0;
	spellData_.debuffType = D_NONE;
}

void EnchantFire::Update()
{
	SpellManager::GetInstance()->SetEnchantFire();

	if (--spellData_.timeAlive <= 0) {
		Dead();
	}
}

void EnchantFire::Fire()
{
	spellData_.timeAlive = TIME_ALIVE;
}

bool EnchantFire::GetActive()
{
	if (spellData_.timeAlive) {
		return true;
	}
	return false;
}
