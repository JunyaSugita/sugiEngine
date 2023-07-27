#pragma once
#include "GrovalSetting.h"

class EnchantFire {
public:
	void Initialize();
	void Update();

	void Fire();
	bool GetActive();

public:
	const int32_t TIME_ALIVE = 15 * 60;	//魔法が消えるまでの時間

private:
	int32_t timer_;
};