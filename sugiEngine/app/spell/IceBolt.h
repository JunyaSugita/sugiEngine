#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseObj.h"
#include "BaseCol.h"

class IceBolt {
public:
	void Initialize(Vector3 pos, Vector3 vec);
	void Update();
	void Draw();

	void Fire();

	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	};

	void SetIsHit() {
		isHit_ = true;
	}

	BoxCol GetBoxCol() {
		return col_.col;
	}

private:
	void SetCol();
	void WorldTransUpdate();

public:
	const float SPEED_MOVE = 0.5f;	//���@�̃X�s�[�h
	const int32_t TIME_ALIVE = 5 * 60;	//���@��������܂ł̎���

private:
	BaseObj obj_;

	//�����蔻��
	BaseCol col_;

	//�i�s����
	Vector3 vec_;

	//�\���t���O
	bool isDead_;
	//������܂ł̎��ԊǗ�
	int32_t time_;
	//�����鎞��Alpha
	float alpha_;

	//���������t���O
	bool isHit_;
};