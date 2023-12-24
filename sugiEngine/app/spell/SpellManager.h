/**
 * @file SpellManager.h
 * @brief 魔法を管理するマネージャー
 */

#pragma once
#include "BaseSpell.h"
#include "FireBall.h"
#include "MagicMissile.h"
#include "IceBolt.h"
#include "ChainLightning.h"
#include "EnchantFire.h"
#include "Flame.h"

enum MAGIC {
	FIRE_BALL,
	MAGIC_MISSILE,
	ICE_BOLT,
	CHAIN_LIGHTNING,
	ENCHANT_FIRE,
	FLAME,
};

enum SPELL_TYPE {
	TYPE_NONE,
	TYPE_FIRE,
	TYPE_THUNDER,
	TYPE_ICE,
	TYPE_DARK,
	TYPE_END,
};

class SpellManager final{
private:
	SpellManager() = default;
	~SpellManager() = default;

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

	void ChargeIceBolt();
	void FireIceBolt();

	void ChargeChainLightning();
	void FireChainLightning();

	void ChargeEnchantFire();
	void FireEnchantFire();

	void ChargeFlame();
	void FireFlame();

	void ResetChargeTime() {
		chargeTime_ = 0;
	}
	float ChargePercent();

	float UsePercent() {
		return useTime_ / maxCharge_;
	}

	float GetMaxCharge() {
		return maxCharge_;
	}

	std::vector<BaseSpell*> GetSpellsCol() {
		spellsList_.clear();
		for (std::unique_ptr<BaseSpell>& spell : spells_) {
			spellsList_.push_back(spell.get());
		}
		return spellsList_;
	}

	std::vector<ChainLightning*> GetChainLightningsCol() {
		chainLightningsList_.clear();
		for (std::unique_ptr<ChainLightning>& chainLightning : chainLightnings_) {
			chainLightningsList_.push_back(chainLightning.get());
		}
		return chainLightningsList_;
	}

	void SetEnchantFire() {
		isModeEnchantFire_ = true;
	}
	bool GetActiveEnchantFire() {
		return isModeEnchantFire_;
	}

	bool GetIsUseSpell();

	int32_t GetSpellType(int32_t spell);

public:
	//ファイアーボール
	const float TIME_CHARGE_FIREBALL = 1.0f * 60;
	const float TIME_FIRE_FIREBALL = 0.3f * 60;
	//マジックミサイル
	const float TIME_CHARGE_MAGICMISSILE = 1.0f * 60;
	const float TIME_FIRE_MAGICMISSILE = 2.0f * 60;
	//アイスボルト
	const float TIME_CHARGE_ICEBOLT = 1.2f * 60;
	const float TIME_FIRE_ICEBOLT = 0.3f * 60;
	//チェインライトニング
	const float TIME_CHARGE_CHAINLIGHTNING = 1.8f * 60;
	const float TIME_FIRE_CHAINLIGHTNING = 0.3f * 60;
	//エンチャントファイア
	const float TIME_CHARGE_ENCHANTFIRE = 3.0f * 60;
	const float TIME_FIRE_ENCHANTFIRE = 0.5f * 60;
	//火炎放射
	const float TIME_CHARGE_FLAME = 2.0f * 60;
	const float TIME_FIRE_FLAME = 3.0f * 60;

private:
	float maxCharge_;
	float chargeTime_;
	float useTime_;

	bool isUseFireBall_;
	bool isUseMagicMissile_;
	bool isUseIceBolt_;
	bool isUseChainLightning_;
	bool isUseEnchantFire_;
	bool isUseFlame_;

	std::list<std::unique_ptr<BaseSpell>> spells_;
	std::vector<BaseSpell*> spellsList_;

	std::list<std::unique_ptr<ChainLightning>> chainLightnings_;
	std::vector<ChainLightning*> chainLightningsList_;

	bool isModeEnchantFire_;
};
