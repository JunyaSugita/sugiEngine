/**
 * @file Fly.h
 * @brief 飛行している敵
 */

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
	void WeakBodyColor() override;

private:
	const float SPEED_MOVE = 0.15f;
	const float SPEED_ANGLE = 5;
	const float MAX_HP = 25;
	const float HEIGHT_COL = 2.2f;

private:
	//体の色
	const Vector4 COLOR_BODY = { 0.1f,0.1f,0.1f,1 };
	const Vector4 COLOR_WEAK_BODY = { 0.2f,0.2f,0.0f,1 };
	//飛ぶ高さ
	const float FLY_Y = 10;
	const float SPEED_DROP = 0.5f;
	//羽のふり幅
	const float LEN_WING = 50;
	//羽の速度
	const float SPEED_WING = 0.25f;
	//墜落速度
	const float SPEED_DROP_DOWN = 0.2f;

	//腕
	BaseObj wingL_;
	BaseObj wingR_;

	static std::unique_ptr<Model> sEyeModel_;
};