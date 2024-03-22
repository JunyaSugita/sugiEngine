/**
 * @file ChainLightning.h
 * @brief 伝播する雷呪文
 */

#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseObj.h"
#include "BaseCol.h"
#include "BaseSpell.h"

class ChainLightning{
public:
	void Initialize(const Vector3& pos, const Vector3& vec);
	void Update();
	void Draw();
	//発射
	void Fire();

	//ゲッターセッター
	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	};
	void SetIsHit() {
		isHit_ = true;
	}

private:
	void WorldTransUpdate();

public:
	static const int32_t TIME_STAN = 2;

private:
	//色
	const Vector4 COLOR = { 0.5f,0.5f,1,0.5f };
	//最大距離
	const int32_t MAX_LENGE = 50;

	BaseObj obj_;

	//進行方向
	Vector3 vec_;

	//表示フラグ
	bool isDead_;
	//消えるまでの時間管理
	int32_t time_;
	//消える時のAlpha
	float alpha_;

	//当たったフラグ
	bool isHit_;

	SpellData spellData_;
};
