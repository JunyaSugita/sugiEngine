#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "Object3d.h"

class FireBall {
public:
	static void OneTimeInitialize();
	void Initialize(Vector3 pos, Vector3 vec);
	void Update();
	void Draw();

	void Fire();

	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	};

private:
	void WorldTransUpdate();
	void SetWorldTrans();

	void Explode();

private:
	static std::unique_ptr<Model> sModel_;

public:
	const float SPEED_MOVE = 1.0f;

private:
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//進行方向
	Vector3 vec_;

	//表示フラグ
	bool isDead_;
	//当たったフラグ
	bool isHit_;

	std::unique_ptr<Object3d> obj_;
};