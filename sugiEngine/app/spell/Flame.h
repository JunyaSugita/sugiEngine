#pragma once
#include "BaseSpell.h"

class Flame : public BaseSpell {
public:
	void Initialize(Vector3 pos, Vector3 vec) override;
	void Update()override;
	void Draw()override;

public:
	const float SPEED_MOVE = 0.4f;	//���@�̃X�s�[�h
	const int32_t TIME_ALIVE = 1 * 60;	//���@��������܂ł̎���

};