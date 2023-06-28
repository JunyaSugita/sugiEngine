#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"

class Player final{
private:
	Player();
	~Player();
public:
	//コピーコンストラクタ無効
	Player(const Player& obj) = delete;
	//代入演算子を無効
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

	//worldTrans
	WorldTransform GetWorldTrans() {
		return worldTrans_;
	}

private:
	void Move();
	void CameraMove();
	void Attack();
	void WorldTransUpdate();

private:
	const Vector3 CAMERA_EYE = { 0.0f,5.0f,0.0f };//プレイヤーの目線調整
	const float SPEED_MOVE = 0.5f;	//プレイヤーのスピード
	const float SPEED_CAMERA = 3.0f;	//カメラのスピード
	const float TIME_ATTACK_NORMAL = 5.0f * 60.0f;	//通常攻撃のスピード
	const float TIME_ATTACK_START_NORMAL = 2.0f * 60.0f;//通常攻撃開始から攻撃判定が出るまでの時間
	const float TIME_ATTACK_END_NORMAL = 4.5f * 60.0f;//通常攻撃開始から攻撃判定が無くなるまでの時間

private:
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;
	Vector2 cameraAngle_;//カメラ角度
	Vector3 frontVec_;//正面ベクトル
	Vector3 rightVec_;//右ベクトル

	int32_t life_;//体力

	bool isAttack_;//攻撃フラグ
	float attackTime_;//攻撃時間
};

float Radian(float r);
