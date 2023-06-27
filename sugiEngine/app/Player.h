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
	const float CAMERA_Y = 2.0f;//プレイヤーの目線調整
	const float SPEED = 0.5f;	//プレイヤーのスピード

private:
	Vector3 pos_;
	Vector2 cameraAngle_;
	Vector3 frontVec_;
	Vector3 rightVec_;
};