#pragma once
#include "FireBall.h"
#include "MagicMissile.h"

enum MAGIC {
	FIRE_BALL,
	MAGIC_MISSILE,
};


enum DEBUFF
{
	NONE,
	FIRE,
	THUNDER,
};

class SpellManager {
private:
	SpellManager();
	~SpellManager();

public:
	//コピーコンストラクタ無効
	SpellManager(const SpellManager& obj) = delete;
	//代入演算子を無効
	SpellManager& operator=(const SpellManager& obj) = delete;

	static SpellManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	void ChargeFireBall();
	void FireFireBall();

	void ChargeMagicMissile();
	void FireMagicMissile();

	void ResetChargeTime() {
		chargeTime_ = 0;
	}
	float ChargePercent();

	float UsePercent() {
		return useTime_ / maxCharge_;
	}

	std::vector<FireBall*> GetFireBallsCol() {
		fireBallsList_.clear();
		for (unique_ptr<FireBall>& fireBall : fireBalls_) {
			fireBallsList_.push_back(fireBall.get());
		}
		return fireBallsList_;
	}

	std::vector<MagicMissile*> GetMagicMissilesCol() {
		magicMissilesList_.clear();
		for (unique_ptr<MagicMissile>& magicMissile : magicMissiles_) {
			magicMissilesList_.push_back(magicMissile.get());
		}
		return magicMissilesList_;
	}

	bool GetIsUseSpell();

public:
	//ファイアーボール
	const float TIME_CHARGE_FIREBALL = 1.0f * 60;
	const float TIME_FIRE_FIREBALL = 0.3f * 60;
	//マジックミサイル
	const float TIME_CHARGE_MAGICMISSILE = 2.5f * 60;
	const float TIME_FIRE_MAGICMISSILE = 2.0f * 60;
private:
	float maxCharge_;
	float chargeTime_;
	float useTime_;

	bool isUseFireBall_;
	bool isUseMagicMissile_;

	std::list<std::unique_ptr<FireBall>> fireBalls_;
	std::vector<FireBall*> fireBallsList_;

	std::list<std::unique_ptr<MagicMissile>> magicMissiles_;
	std::vector<MagicMissile*> magicMissilesList_;
};