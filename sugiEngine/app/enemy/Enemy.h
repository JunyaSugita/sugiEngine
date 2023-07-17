#pragma once
#include "Sugimath.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Object3d.h"
#include "ColliderManager.h"

struct DebuffM {
	bool isFire;
	int32_t fireTime;
	bool isThunder;
	int32_t thunderTime;
	bool isIce;
	int32_t iceTime;
};

class Enemy{
public:
	static void OneTimeInitialize();
	void Initialize(Vector3 pos = Vector3(0,0,0));
	void Update();
	void Draw();

	Vector3 GetPos() {
		return pos_;
	}


	void SetCol(Vector3 vec) {
		boxCol_.pos = vec;
	}
	void AddCol(Vector3 vec) {
		boxCol_.pos += vec;
	}
	void AddColX(float x) {
		boxCol_.pos.x += x;
	}
	void AddColZ(float z) {
		boxCol_.pos.z += z;
	}

	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	}

	void SetIsHit(int32_t subLife = 1, int32_t effectNum = 0);
	void SetDebuff(uint8_t debuff,uint32_t time);
	void ResetIsHit() {
		isHit_ = false;
	}

	BoxCol GetBoxCol() {
		return boxCol_;
	}

	bool isDebuff();
	bool isCanMove();

	void SetIsStop() {
		isStop_ = true;
	}

private:
	void SetCol();
	void WorldTransUpdate();
	void SetWorldTrans();
	void SetAngleToPlayer();
	void GetPlayer();
	void Move();
	void SubLife(int32_t subLife, int32_t effectNum);
	void UpdateDebuff();
	void SetShake();
	void ResetShake();
	float GetSlow();

private:
	const float SPEED_MOVE = 0.15f;
	const float SPEED_ANGLE = 1;
	const float RADIAN = 180;
	const Vector2 UP = { 0,-1 };
	const uint32_t MAX_HP = 25;

private:
	static std::unique_ptr<Model> sEyeModel_;
	static std::unique_ptr<Model> sColModel_;
private:
	//�{��
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//�����蔻��
	BoxCol boxCol_;

	//�̗�
	int32_t life_;

	//���񂾂��ǂ���
	bool isDead_;

	//�v���C���[�Ƃ̋������L�^
	Vector2 toPlayer;

	//���i�q�b�g�̉���t���O
	bool isHit_;

	//�f�o�t
	DebuffM debuff_;

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

	//�����蔻��
	WorldTransform colWorldTrans_;
	//�����蔻��̃��f���֌W
	std::unique_ptr<Object3d> colObj_;

	//�����~�߂邩
	bool isStop_;
};