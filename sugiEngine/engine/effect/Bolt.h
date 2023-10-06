#pragma once
#include "GrovalSetting.h"
#include "BaseObj.h"

class Bolt {
public:
	void Initialize(const Vector3& posS, const Vector3& posE, const Vector3& rot, Vector4 color);
	void Update();
	void Draw();

	bool IsDead() const {
		return isDead_;
	}

private:
	static const int NUM = 50;
	//座標
	BaseObj obj_[NUM];
	//回転
	Vector3 rotate_;
	//時間
	float timer_;
	//消滅フラグ
	bool isDead_;
};
