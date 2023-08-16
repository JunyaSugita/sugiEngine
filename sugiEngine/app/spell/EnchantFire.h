#pragma once
#include "GrovalSetting.h"
#include "BaseSpell.h"

class EnchantFire : public BaseSpell{
public:
	void Initialize(Vector3 pos, Vector3 vec)override;
	void Update()override;
	void Fire()override;

	bool GetActive();

public:
	const int32_t TIME_ALIVE = 15 * 60;	//–‚–@‚ªÁ‚¦‚é‚Ü‚Å‚ÌŠÔ
};