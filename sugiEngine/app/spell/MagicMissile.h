#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseObj.h"
#include "ColliderManager.h"

class MagicMissile {
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

	void Explode();

public:
	const float SPEED_MOVE = 1.0f;	//���@�̃X�s�[�h
	const int32_t TIME_ALIVE = 10 * 60;

private:
	BaseObj obj_;

	//�����蔻��
	BoxCol boxCol_;

	//�i�s����
	Vector3 vec_;

	//�\���t���O
	bool isDead_;
	//������܂ł̎���
	int32_t time_;
	//���������t���O
	bool isHit_;

	//�����蔻��
	WorldTransform colWorldTrans_;
	//�����蔻��̃��f���֌W
	std::unique_ptr<Object3d> colObj_;
};
