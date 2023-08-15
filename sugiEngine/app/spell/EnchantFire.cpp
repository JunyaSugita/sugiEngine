#include "EnchantFire.h"

void EnchantFire::Initialize(Vector3 pos, Vector3 vec)
{
	time_ = 0;
}

void EnchantFire::Update()
{
	if (GetActive()) {
		time_--;
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
