#pragma once
#include "SugiMath.h"
#include "BaseSpell.h"

class MagicMissile : public BaseSpell{
public:
	void Initialize(Vector3 pos, Vector3 vec) override;
	void Update()override;

public:
	const float SPEED_MOVE = 1.0f;	//魔法のスピード
	const int32_t TIME_ALIVE = 10 * 60;
};
