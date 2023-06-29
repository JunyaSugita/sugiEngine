#pragma once
#include "Sugimath.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Object3d.h"

class Enemy{
public:
	static void OneTimeInitialize();
	void Initialize(Vector3 pos = Vector3(0,0,0));
	void Update();
	void Draw();

	Vector3 GetPos() {
		return pos_;
	}

	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	}

	void SetIsHit();
	void ResetIsHit() {
		isHit_ = false;
	}

private:
	void WorldTransUpdate();
	void SetWorldTrans();
	void SetAngleToPlayer();
	void GetPlayer();
	void Move();
	void SubLife();

private:
	const float SPEED_MOVE = 0.05f;
	const float SPEED_ANGLE = 1;
	const float RADIAN = 180;
	const Vector2 UP = { 0,-1 };

private:
	static std::unique_ptr<Model> sEyeModel_;
private:
	//�{��
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//�̗�
	int32_t life_;

	//���񂾂��ǂ���
	bool isDead_;

	//�v���C���[�Ƃ̋������L�^
	Vector2 toPlayer;

	//���i�q�b�g�̉���t���O
	bool isHit_;

	//�{�̂̃��f���֌W
	std::unique_ptr<Model> model_;
	std::unique_ptr<Object3d> obj_;

	//��
	WorldTransform eyeWorldTrans_;
	Vector3 eyePos_;
	Vector3 eyeRot_;
	Vector3 eyeScale_;

	//�ڂ̃��f���֌W
	
	std::unique_ptr<Object3d> eyeObj_;
};