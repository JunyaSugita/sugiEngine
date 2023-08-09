#pragma once
#include "BaseEnemy.h"

class Slime : public BaseEnemy {
public:
	static void StaticInitialize();
	void Initialize(Vector3 pos) override;
	void Draw() override;
	void WorldTransUpdate() override;

private:
	void Move() override;
	void Attack() override;


};