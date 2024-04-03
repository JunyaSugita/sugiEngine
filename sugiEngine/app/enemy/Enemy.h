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
	//進むスピード
	const float SPEED_MOVE = 0.15f;
	//振り向き速度
	const float SPEED_ANGLE = 5;
	//体力
	const float MAX_HP = 25;
	//Y軸の当たり判定の大きさ
	const float SIZE_HEIGHT_COL = 3.0f;
	//Y軸の当たり判定のズレ調整
	const float GAP_HEIGHT_COL = 2.2f;
	//攻撃間隔の値の減少値
	const float SUB_TIME_ATTACK = 0.025f;
	//ダウンした時に後ろに倒れるスピード
	const float SPEED_DOWN = 5;
	//攻撃時の腕のふり幅
	const float ROT_ARM = 50;

private:
	//目
	BaseObj eyeObj_;

	//腕
	BaseObj armL_;
	BaseObj armR_;

	//目のモデル
	static std::unique_ptr<Model> sEyeModel_;
};
