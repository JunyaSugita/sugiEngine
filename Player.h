#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"

class Player
{
public:
	void Initialize();
	void Update(Input* input);
	void Draw();

private:
	//固定値
	const float GRAVITY = -0.5f;

	//モデルデータ
	Model* modelPlayer_;
	Object3d* objectPlayer_;

	//プレイヤーの情報
	WorldTransform player_;
	float speed_;
	float jumpPow_;
	bool isJump_;
	Vector3 way_;
	int dash_;
	bool isDash_;
};

