#pragma once
#include "BaseCol.h"
#include "WorldTransform.h"

class Bullet : public BaseCol
{
public:
	void Initialize(Vector3 pos);
	void Update();
	void Draw();

	//ゲッターセッター
	bool GetIsHit()override{
		return isHit_;
	};
	void SetIsHit()override{
		isHit_ = true;
	};
	bool GetIsDead() {
		return isHit_;
	}

private:
	//スピード
	const float SPEED = 1.5f;

	//方向
	Vector3 vec_;
	//当たったかのフラグ
	bool isHit_;

	//物体
	BaseObj obj_;
};

