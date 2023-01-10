#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "Map.h"

enum {
	LT,
	LB,
	RT,
	RB
};

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

	bool isJumpNow = false;
	int isWallJumpL_ = 0;
	int isWallJumpR_ = 0;

	//残像
	WorldTransform shadow_[CONST_SHADOW];
	float shadowAlpha_[CONST_SHADOW];

	//当たり判定用
	Vector2 LT_;
	Vector2 LB_;
	Vector2 RT_;
	Vector2 RB_;

	WorldTransform trans_[4];

	//プレイヤー生存
	bool isDead_;
	//ゴール判定
	bool isGoal_;

	//マップ
	int mapNum = 0;
};

