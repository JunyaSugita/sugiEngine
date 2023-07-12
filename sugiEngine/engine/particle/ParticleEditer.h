#pragma once
#include "SugiMath.h"

class ParticleEditer {
public:
	void Initialize();
	void Update();

	void PopParticle(uint8_t num);

	bool GetIsEdit(uint8_t num) {
		return isEdit_[num];
	}

public:
	static const uint8_t particleNum = 3;

private:
	bool isEdit_[particleNum];
	int32_t life_[particleNum];
	float scale_[particleNum][2];
	float color_[particleNum][4];
	float pos_[particleNum][3];
	float move_[particleNum][3];
	float moveRand_[particleNum][3];
	float gravity_[particleNum][3];
};