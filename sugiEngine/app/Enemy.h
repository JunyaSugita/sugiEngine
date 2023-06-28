#pragma once
#include "Sugimath.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Object3d.h"

class Enemy{
public:
	void Initialize();
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
	//本体
	WorldTransform worldTransform_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//体力
	int32_t life_;

	//死んだかどうか
	bool isDead_;

	//プレイヤーとの距離を記録
	Vector2 toPlayer;

	//多段ヒットの回避フラグ
	bool isHit_;

	//本体のモデル関係
	std::unique_ptr<Model> model_;
	std::unique_ptr<Object3d> obj_;

	//目
	WorldTransform eyeWorldTransform_;
	Vector3 eyePos_;
	Vector3 eyeRot_;
	Vector3 eyeScale_;

	//目のモデル関係
	std::unique_ptr<Model> eyeModel_;
	std::unique_ptr<Object3d> eyeObj_;
};