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
	//���W
	BaseObj obj_;
	//�ړ�
	Vector3 move_;
	//��]
	Vector3 rotate_;
	//���Ńt���O
	bool isDead_;
};
