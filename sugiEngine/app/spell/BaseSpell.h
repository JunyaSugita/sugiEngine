/**
 * @file BaseSpell.h
 * @brief 魔法のベース
 */

#pragma once
#include "GlobalSetting.h"
#include "BaseObj.h"
#include "BaseCol.h"

enum SpellType {
	SHOT,
	BUFF
};

enum DebuffType {
	D_NONE,
	D_FIRE,
	D_SLOW,
	D_STAN,
};

class BaseSpell :public BaseCol{
public:
	virtual void Initialize(Vector3 pos, Vector3 vec = Vector3());
	virtual void Update();
	virtual void Draw();
	virtual void WorldTransUpdate();

	//発射
	virtual void Fire();
	//着弾時効果
	virtual void Explode();

	void OnCollision(BaseCol* a)override;

public:
	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	};
	bool GetIsHit() override{
		return isHit_;
	}
	void SetIsHit() override {
		isHit_ = true;
	}
	Col GetBoxCol() {
		return col_;
	}
	float GetDamage() override{
		return damage_;
	}
	int32_t GetDebuff() override{
		return debuffType_;
	}
	bool GetIsCalcCol();

	void Dead();

protected:
	//魔法が出る位置
	const float SPELL_LENGE = 3;

	//位置や実体
	BaseObj obj_;
	//表示フラグ
	bool isDead_;
	//消えるまでの時間管理
	int32_t time_;
	//当たったフラグ
	bool isHit_;
	//進行方向
	Vector3 vec_ = {0,0,0};
	//移動スピード
	float moveSpeed_ = 0;
	//呪文のタイプ
	int32_t spellType_;

	//ダメージ(回復)
	float damage_ = 0;
	//デバフ
	int32_t debuffType_ = D_NONE;
};
