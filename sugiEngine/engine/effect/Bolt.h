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
	//���W
	BaseObj obj_[NUM];
	//��]
	Vector3 rotate_;
	//����
	float timer_;
	//���Ńt���O
	bool isDead_;
};