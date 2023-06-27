#pragma once
#include "SugiMath.h"

class Player{
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

private:
	const float CAMERA_Y = 2.0f;//�v���C���[�̖ڐ�����
	const float SPEED = 0.5f;	//�v���C���[�̃X�s�[�h

private:
	Vector3 pos_;
	Vector2 cameraAngle_;
	Vector3 frontVec_;
	Vector3 rightVec_;
};