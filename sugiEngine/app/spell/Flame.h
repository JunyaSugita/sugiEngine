#pragma once
#include "BaseSpell.h"

class Flame : public BaseSpell {
public:
	void Initialize(Vector3 pos, Vector3 vec) override;
	void Update()override;
	void Draw()override;

public:
	const float SPEED_MOVE = 0.4f;	//魔法のスピード
	const int32_t TIME_ALIVE = 1 * 60;	//魔法が消えるまでの時間

};