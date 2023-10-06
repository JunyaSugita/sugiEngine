#pragma once
#include "ColliderManager.h"
#include "BaseObj.h"

class BaseCol {
public:
	void Initialize();
	void Update();
	void Draw();

	void SetCol(Vector3 pos);
	void SetOldCol();

public:
	BoxCol col;
	BoxCol oldCol;
	Vector3 gap;

private:
	BaseObj colObj_;
};
