#pragma once
#include "SugiMath.h"

class Player final{
private:
	Player();
	~Player();
public:
	//�R�s�[�R���X�g���N�^����
	Player(const Player& obj) = delete;
	//������Z�q�𖳌�
	Player& operator=(const Player& obj) = delete;

	static Player* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	//pos
	Vector3 GetPos() {
		return pos_;
	}
	Vector3 SetPos(const Vector3& pos) {
		pos_ = pos;
	}
	Vector3 SetPosX(float pos) {
		pos_.x = pos;
	}
	Vector3 SetPosY(float pos) {
		pos_.y = pos;
	}
	Vector3 SetPosZ(float pos) {
		pos_.z = pos;
	}

	//life
	uint32_t GetLife() {
		return life_;
	}
	void SubLife() {
		life_--;
	}

private:
	void Move();
	void CameraMove();

private:
	const Vector3 CAMERA_EYE = { 0.0f,5.0f,0.0f };//�v���C���[�̖ڐ�����
	const float SPEED_MOVE = 0.5f;	//�v���C���[�̃X�s�[�h
	const float SPEED_CAMERA = 3.0f;	//�J�����̃X�s�[�h

private:
	Vector3 pos_;
	Vector2 cameraAngle_;
	Vector3 frontVec_;
	Vector3 rightVec_;

	int32_t life_;
};

float Radian(float r);
