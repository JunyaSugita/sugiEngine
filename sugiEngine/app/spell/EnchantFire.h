#pragma once
#include "GrovalSetting.h"

class EnchantFire {
public:
	void Initialize();
	void Update();

	void Fire();
	bool GetActive();

public:
	const int32_t TIME_ALIVE = 15 * 60;	//���@��������܂ł̎���

private:
	int32_t timer_;
};