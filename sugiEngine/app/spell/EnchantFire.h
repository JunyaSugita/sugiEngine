/**
 * @file EnchantFire.h
 * @brief 武器に火属性を付与する呪文
 */

#pragma once
#include "GlobalSetting.h"
#include "BaseSpell.h"

class EnchantFire : public BaseSpell{
public:
	void Initialize(Vector3 pos, Vector3 vec)override;
	void Update()override;
	void Fire()override;

	//武器に火がついているか
	bool GetActive();

public:
	const int32_t TIME_ALIVE = 15 * 60;	//魔法が消えるまでの時間
	static const int32_t DAMAGE = 10;
	static const int32_t TIME_DEBUFF = 3;
};
