#pragma once
#include "BaseSpell.h"

class IceBolt : public BaseSpell{
public:
	void Initialize(Vector3 pos, Vector3 vec)override;
	void Update()override;

public:
	const float SPEED_MOVE = 0.5f;	//���@�̃X�s�[�h
	const int32_t TIME_ALIVE = 5 * 60;	//���@��������܂ł̎���
};