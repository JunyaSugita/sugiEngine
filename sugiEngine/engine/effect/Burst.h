#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "BaseObj.h"

class Burst {
public:
	void Initialize(const Vector3& pos,Vector4 color, float range,float pow);
	void Update();
	void Draw();

	bool IsDead() const {
		return isDead_;
	}

private:
	//座標
	BaseObj obj_;
	//移動
	Vector3 move_;
	//回転
	Vector3 rotate_;
	//消滅フラグ
	bool isDead_;
};
