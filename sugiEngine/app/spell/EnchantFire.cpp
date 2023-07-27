#include "EnchantFire.h"

void EnchantFire::Initialize()
{
	timer_ = 0;
}

void EnchantFire::Update()
{
	if (GetActive()) {
		timer_--;
	}
}

void EnchantFire::Fire()
{
	timer_ = TIME_ALIVE;
}

bool EnchantFire::GetActive()
{
	if (timer_) {
		return true;
	}
	return false;
}
