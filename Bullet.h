#pragma once
#include "BaseCol.h"
#include "WorldTransform.h"

class Bullet : public BaseCol
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	WorldTransform worldTrans_;
	Vector3 vec_;
};

