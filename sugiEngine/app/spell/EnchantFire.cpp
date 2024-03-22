#include "EnchantFire.h"
#include "SpellManager.h"
#include "Status.h"

void EnchantFire::Initialize(const Vector3& pos, const Vector3& vec)
{
	pos;
	vec;

	spellData_ = Status::GetInstance()->GetSpellData(ENCHANT_FIRE);
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
	spellData_.timeAlive = spellData_.timeAlive;
}

bool EnchantFire::GetActive()
{
	if (spellData_.timeAlive) {
		return true;
	}
	return false;
}
