#pragma once
#include "BaseCol.h"
#include "WorldTransform.h"

class Bullet : public BaseCol
{
public:
	void Initialize(Vector3 pos);
	void Update();
	void Draw();

	bool GetIsHit()override;
	void SetIsHit()override;

	bool GetIsDead() {
		return isHit_;
	}

private:
	const float SPEED = 1.5f;

	Vector3 vec_;
	bool isHit_;

	BaseObj obj_;
};

