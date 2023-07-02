#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "Object3d.h"
#include "ColliderManager.h"

class MagicMissile {
public:
	static void OneTimeInitialize();
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
	void SetWorldTrans();

	void Explode();

private:
	static std::unique_ptr<Model> sModel_;
	static std::unique_ptr<Model> sColModel_;

public:
	const float SPEED_MOVE = 1.0f;

private:
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//�����蔻��
	BoxCol boxCol_;

	//�i�s����
	Vector3 vec_;

	//�\���t���O
	bool isDead_;
	//���������t���O
	bool isHit_;

	std::unique_ptr<Object3d> obj_;

	//�����蔻��
	WorldTransform colWorldTrans_;
	//�����蔻��̃��f���֌W
	std::unique_ptr<Object3d> colObj_;
};
