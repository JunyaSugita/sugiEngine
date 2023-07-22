#pragma once
#include "GrovalSetting.h"
#include "Gauge.h"

class GameOver {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	Gauge gauge_;

	int32_t maxHp_;
	int32_t nowHp_;

	float blur_;
};