#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseObj.h"
#include "ColliderManager.h"

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
		return boxCol_;
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
	BoxCol boxCol_;

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

	//�����蔻��
	WorldTransform colWorldTrans_;
	//�����蔻��̃��f���֌W
	std::unique_ptr<Object3d> colObj_;
};