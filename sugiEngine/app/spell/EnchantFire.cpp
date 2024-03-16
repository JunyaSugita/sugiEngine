#include "EnchantFire.h"
#include "SpellManager.h"

void EnchantFire::Initialize(const Vector3& pos, const Vector3& vec)
{
	pos;
	vec;
	spellData_.time = TIME_ALIVE;
	spellData_.spellType = BUFF;
	spellData_.damage = 0;
	spellData_.debuffType = D_NONE;
}

void EnchantFire::Update()
{
	SpellManager::GetInstance()->SetEnchantFire();

	if (--spellData_.time <= 0) {
		Dead();
	}
}

void EnchantFire::Fire()
{
	spellData_.time = TIME_ALIVE;
}

bool EnchantFire::GetActive()
{
	if (spellData_.time) {
		return true;
	}
	return false;
}
