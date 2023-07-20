#pragma once
#include "GrovalSetting.h"
#include "Gauge.h"

class ClearChecker {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	int32_t maxEnemy_;
	int32_t nowEnemy_;

	Gauge gauge_;
};