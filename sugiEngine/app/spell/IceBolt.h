#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "Object3d.h"
#include "ColliderManager.h"

class IceBolt {
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

	void SetIsHit() {
		isHit_ = true;
	}

	BoxCol GetBoxCol() {
		return boxCol_;
	}

private:
	void SetCol();
	void WorldTransUpdate();
	void SetWorldTrans();

private:
	static std::unique_ptr<Model> sModel_;
	static std::unique_ptr<Model> sColModel_;

public:
	const float SPEED_MOVE = 0.5f;	//魔法のスピード
	const int32_t TIME_ALIVE = 5 * 60;	//魔法が消えるまでの時間

private:
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//当たり判定
	BoxCol boxCol_;

	//進行方向
	Vector3 vec_;

	//表示フラグ
	bool isDead_;
	//消えるまでの時間管理
	int32_t time_;
	//消える時のAlpha
	float alpha_;


	//当たったフラグ
	bool isHit_;

	std::unique_ptr<Object3d> obj_;

	//当たり判定
	WorldTransform colWorldTrans_;
	//当たり判定のモデル関係
	std::unique_ptr<Object3d> colObj_;
};