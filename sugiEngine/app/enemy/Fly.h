#pragma once
#include "BaseEnemy.h"
#include "BaseObj.h"

class Fly : public BaseEnemy {
public:
	void Initialize(std::string name, Vector3 pos) override;
	void Draw() override;
	void WorldTransUpdate() override;

private:
	void Move() override;
	void DontMoveUpdate() override;
	void Attack() override;
	void Down() override;

private:
	const float SPEED_MOVE = 0.15f;
	const float SPEED_ANGLE = 5;
	const uint32_t MAX_HP = 25;
	const float HEIGHT_COL = 2.2f;

private:
	//腕
	BaseObj armL_;
	BaseObj armR_;

	static std::unique_ptr<Model> sEyeModel_;
};