#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "Map.h"

class Player
{
public:
	void Initialize();
	void Update(Input* input,Map* map);
	void Draw();
	void Delete();

	void SetShadow(Vector3 pos);

private:
	//固定値
	const float GRAVITY = -0.5f;
	static const int CONST_SHADOW = 10;
	//モデルデータ
	Model* modelPlayer_;
	Model* modelShadow_[CONST_SHADOW];
	Object3d* objectPlayer_;
	Object3d* objectShadow_[CONST_SHADOW];

	//プレイヤーの情報
	WorldTransform player_;
	float speed_;
	float jumpPow_;
	bool isJump_;
	Vector3 way_;
	int dash_;
	bool isDash_;
	bool isGrab_;

	//残像
	WorldTransform shadow_[CONST_SHADOW];
	float shadowAlpha_[CONST_SHADOW];

	//当たり判定用
	Vector2 LT_;
	Vector2 LB_;
	Vector2 RT_;
	Vector2 RB_;
};

