#pragma once
#include "SugiMath.h"

class ParticleEditer {
public:
	void Initialize();
	void Update();

private:
	bool isEdit = false;
	int32_t life_;
	float scale_[2];
	float color_[4];
	float pos_[3];
	float move_[3];
	float gravity_[3];
};