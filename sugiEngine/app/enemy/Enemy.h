/**
 * @file Enemy.h
 * @brief 人型の敵
 */
#pragma once
#include "BaseEnemy.h"
#include "BaseObj.h"

class Enemy : public BaseEnemy{
public:
	void Initialize(const std::string& name, const Vector3& pos) override;
	void Draw() override;
	void WorldTransUpdate() override;

private:
	void Move() override;
	void Attack() override;
	void Down() override;
	
private:
	const float SPEED_MOVE = 0.15f;
	const float SPEED_ANGLE = 5;
	const float MAX_HP = 25;
	const float HEIGHT_COL = 2.2f;
	const float SUB_TIME_ATTACK = 0.025f;
	const float SPEED_DOWN = 5;
	const float ROT_ARM = 50;

private:
	//目
	BaseObj eyeObj_;

	//腕
	BaseObj wingL_;
	BaseObj wingR_;

	static std::unique_ptr<Model> sEyeModel_;
};
