#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseSpell.h"

class FireBall : public BaseSpell{
public:
	void Initialize(Vector3 pos, Vector3 vec = Vector3()) override;
	void Update()override;
	void Draw()override;

	void Explode()override;

public:
	const float SPEED_MOVE = 1.0f;	//���@�̃X�s�[�h
	const int32_t TIME_ALIVE = 10 * 60;	//���@��������܂ł̎���

private:
	//�����鎞��Alpha
	float alpha_;
};