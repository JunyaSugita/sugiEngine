#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseObj.h"
#include "ColliderManager.h"

class IceBolt {
public:
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

public:
	const float SPEED_MOVE = 0.5f;	//魔法のスピード
	const int32_t TIME_ALIVE = 5 * 60;	//魔法が消えるまでの時間

private:
	BaseObj obj_;

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

	//当たり判定
	WorldTransform colWorldTrans_;
	//当たり判定のモデル関係
	std::unique_ptr<Object3d> colObj_;
};