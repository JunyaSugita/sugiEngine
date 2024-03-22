/**
 * @file IceBolt.h
 * @brief 氷柱を飛ばす魔法
 */

#pragma once
#include "BaseSpell.h"

class IceBolt : public BaseSpell{
public:
	void Initialize(const Vector3& pos, const Vector3& vec)override;
	void Update()override;
	void Draw()override;
};
