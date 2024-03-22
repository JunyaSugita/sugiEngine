/**
 * @file Wind.h
 * @brief 竜巻を飛ばす魔法
 */

#pragma once
#include "SugiMath.h"
#include "BaseSpell.h"
#include "LightGroup.h"

class Wind : public BaseSpell {
public:
	void Initialize(const Vector3& pos, const Vector3& vec) override;
	void Update()override;
	void Draw()override;

	float GetKnockBackXZ()override;
	float GetKnockBackY()override;

	//ライトグループセット
	static void SetLight(LightGroup* lightGroup) {
		lightGroup_ = lightGroup;
	}

public:
	static LightGroup* lightGroup_;
	int32_t useLightNum_;

	static const int32_t NUM_EFFECT = 50;

	WorldTransform w_[NUM_EFFECT];
	float angle_[NUM_EFFECT];
};

