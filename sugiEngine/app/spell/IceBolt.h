/**
 * @file IceBolt.h
 * @brief 氷柱を飛ばす魔法
 */

#pragma once
#include "BaseSpell.h"

class IceBolt : public BaseSpell{
public:
	void Initialize(Vector3 pos, Vector3 vec)override;
	void Update()override;

public:
	const float SPEED_MOVE = 0.5f;	//魔法のスピード
	const int32_t TIME_ALIVE = 5 * 60;	//魔法が消えるまでの時間
	const int32_t DAMAGE = 15;
};
