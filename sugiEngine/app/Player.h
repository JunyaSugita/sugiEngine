#pragma once
#include "SugiMath.h"

class Player{
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

private:
	void Move();
	void CameraMove();

private:
	const Vector3 CAMERA_EYE = { 0.0f,2.0f,0.0f };//プレイヤーの目線調整
	const float SPEED_MOVE = 0.5f;	//プレイヤーのスピード
	const float SPEED_CAMERA = 1.0f;	//カメラのスピード

private:
	Vector3 pos_;
	Vector2 cameraAngle_;
	Vector3 frontVec_;
	Vector3 rightVec_;
};