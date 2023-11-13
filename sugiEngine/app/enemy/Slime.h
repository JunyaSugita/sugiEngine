#pragma once
#include "BaseEnemy.h"

class Slime : public BaseEnemy {
public:
	static void StaticInitialize();
	void Initialize(std::string name,Vector3 pos) override;
	void Draw() override;
	void WorldTransUpdate() override;

private:
	void Move() override;
	void Attack() override;
	void Down() override;
	void DownHitPlayer()override;

private:
	const float SPEED_MOVE = 0.15f;
	const float SPEED_ANGLE = 5;
	const uint32_t MAX_HP = 25;
	const float HEIGHT_COL = 1.0f;

private:
	float timer_;

};
