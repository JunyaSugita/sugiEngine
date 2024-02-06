/**
 * @file FireBall.h
 * @brief 火球を飛ばす魔法
 */

#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseSpell.h"
#include "LightGroup.h"

class FireBall : public BaseSpell{
public:
	void Initialize(const Vector3& pos, const Vector3& vec) override;
	void Update()override;
	void Draw()override;

	void Explode()override;

	//ライトグループセット
	static void SetLight(LightGroup* lightGroup) {
		lightGroup_ = lightGroup;
	}

public:
	const float SPEED_MOVE = 1.0f;	//魔法のスピード
	const int32_t TIME_ALIVE = 10 * 60;	//魔法が消えるまでの時間

private:
	//爆発時
	const float SPEED_SIZE_UP_EXPLODE = 1.2f;
	const float SPEED_ALPHA_EXPLODE = 0.03f;
	const float MAX_SIZE_EXPLODE = 10.0f;

	//消える時のAlpha
	float alpha_;

	static LightGroup* lightGroup_;
	int32_t useLightNum_;
};
