#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseSpell.h"

class FireBall : public BaseSpell{
public:
	void Initialize(Vector3 pos, Vector3 vec = Vector3()) override;
	void Update()override;
	void Draw()override;

	void Explode()override;

public:
	const float SPEED_MOVE = 1.0f;	//魔法のスピード
	const int32_t TIME_ALIVE = 10 * 60;	//魔法が消えるまでの時間

private:
	//消える時のAlpha
	float alpha_;
};