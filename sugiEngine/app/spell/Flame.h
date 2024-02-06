/**
 * @file Flame.h
 * @brief 火炎放射する魔法
 */

#pragma once
#include "BaseSpell.h"

class Flame : public BaseSpell {
public:
	void Initialize(const Vector3& pos, const Vector3& vec) override;
	void Update()override;
	void Draw()override;

public:
	const float SPEED_MOVE = 0.4f;	//魔法のスピード
	const int32_t TIME_ALIVE = 1 * 60;	//魔法が消えるまでの時間

};
