/**
 * @file Slime.h
 * @brief スライムの敵
 */
#pragma once
#include "BaseEnemy.h"

class Slime : public BaseEnemy {
public:
	void Initialize(std::string name,Vector3 pos) override;
	void Draw() override;
	void DrawTransparent() override;
	void WorldTransUpdate() override;

private:
	void Move() override;
	void Attack() override;
	void Down() override;
	void DownHitPlayer()override;
	//死んだあと他の敵に当たった時の反応
	DownState GetDownHitEnemy()override;
	void PopDebuffFireParticle()override;

private:
	const float SPEED_MOVE = 0.15f;
	const float SPEED_ANGLE = 5;
	const uint32_t MAX_HP = 25;
	const float HEIGHT_COL = 1.0f;
	//アニメーションの調整
	const float SAVE_ANIME = 0.5f;
	const float SPEED_ANIME_TIMER = 0.1f;
	//死んだ時の最大サイズ
	const float MAX_WIDTH_DOWN = 10;
	const float MAX_HEIGHT_DOWN = 0.2f;
	//死んだ時のぺちゃんこになる速度
	const float SPEED_WIDTH_DOWN = 0.5f;
	const float SPEED_HEIGHT_DOWN = 0.1f;
	//死んだ後のパーティクル範囲
	const float RENGE_DOWN_FIELD = 7.7f;
	//死んだあとプレイヤーに与えるデバフ
	const float SPEED_DOWN_DEBUFF_TO_PLAYER = 0.4f;
	const int32_t DAMAGE_DOWN_FIRE_TO_PLAYER = 10;
	//死んだあと敵に与えるデバフ
	const float SPEED_DOWN_DEBUFF_TO_ENEMY = 0.5f;
	const int32_t DAMAGE_DOWN_FIRE_TO_ENEMY = 1;
	//攻撃時間
	const float SUB_ATTACK_TIMER = 0.025f;

private:
	float timer_;

};
