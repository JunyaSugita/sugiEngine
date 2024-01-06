/**
 * @file GameOver.h
 * @brief ゲームオーバーの表示やHPの管理
 */

#pragma once
#include "GlobalSetting.h"
#include "Gauge.h"

class GameOver {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	//ブラー
	const float MAX_BLUR = 5;
	const float SPEED_BLUR = 0.025f;

	Gauge gauge_;

	int32_t maxHp_;
	int32_t nowHp_;

	float blur_;
};
