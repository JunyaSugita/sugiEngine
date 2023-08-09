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
	//À•W
	BaseObj obj_;
	//ˆÚ“®
	Vector3 move_;
	//‰ñ“]
	Vector3 rotate_;
	//Á–Åƒtƒ‰ƒO
	bool isDead_;
};
