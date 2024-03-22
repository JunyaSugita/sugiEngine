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
};
