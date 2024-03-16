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

struct SpellData {
	int32_t spellType;
	float damage;
	int32_t debuffType;
	float speed;
	int32_t time;
};

class BaseSpell :public BaseCol{
public:
	virtual void Initialize(const Vector3& pos, const Vector3& vec = Vector3());
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
		tempIsHit_ = true;
	}
	Col GetBoxCol() {
		return col_;
	}
	float GetDamage() override{
		return spellData_.damage;
	}
	int32_t GetDebuff() override{
		return spellData_.debuffType;
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
	//当たったフラグ
	bool isHit_;
	bool tempIsHit_;
	//進行方向
	Vector3 vec_ = {0,0,0};

	//呪文のステータス
	SpellData spellData_;
};
