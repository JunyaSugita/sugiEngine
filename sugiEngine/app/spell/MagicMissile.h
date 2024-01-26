/**
 * @file MagicMissile.h
 * @brief 複数の魔法の弾を飛ばす魔法
 */

#pragma once
#include "SugiMath.h"
#include "BaseSpell.h"
#include "LightGroup.h"

class MagicMissile : public BaseSpell{
public:
	void Initialize(Vector3 pos, Vector3 vec) override;
	void Update()override;
	void Draw()override;

	//ライトグループセット
	static void SetLight(LightGroup* lightGroup) {
		lightGroup_ = lightGroup;
	}

public:
	const float SPEED_MOVE = 1.0f;	//魔法のスピード
	const int32_t TIME_ALIVE = 5 * 60;
	const float DAMAGE = 12;

	static LightGroup* lightGroup_;
	int32_t useLightNum_;
};
