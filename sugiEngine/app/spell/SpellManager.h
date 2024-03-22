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

	MAGIC_END
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

	//呪文のチャージと発射
	void ChargeSpell(int32_t spell);

	//ファイアーボールの発射
	void FireFireBall();
	//マジックミサイルの発射
	void FireMagicMissile();
	//アイスボルトの発射
	void FireIceBolt();
	//チェインライトニングの発射
	void FireChainLightning();
	//エンチャントファイアの発射
	void FireEnchantFire();
	//火炎放射の発射
	void FireFlame();

	//ボタンを離した時などにチャージタイムを初期化
	void ResetChargeTime() {
		chargeTime_ = 0;
	}
	//何パーセント溜まっているか
	float ChargePercent();
	float UsePercent() {
		return useTime_ / maxCharge_;
	}
	float GetMaxCharge() {
		return maxCharge_;
	}

	//ライトニング以外の呪文の判定
	std::vector<BaseSpell*> GetSpellsCol() {
		spellsList_.clear();
		for (std::unique_ptr<BaseSpell>& spell : spells_) {
			spellsList_.push_back(spell.get());
		}
		return spellsList_;
	}
	//ライトニングの呪文の判定
	std::vector<ChainLightning*> GetChainLightningsCol() {
		chainLightningsList_.clear();
		for (std::unique_ptr<ChainLightning>& chainLightning : chainLightnings_) {
			chainLightningsList_.push_back(chainLightning.get());
		}
		return chainLightningsList_;
	}
	//エンチャントファイア適応
	void SetEnchantFire() {
		isModeEnchantFire_ = true;
	}
	//エンチャントファイアを使っているか
	bool GetActiveEnchantFire() {
		return isModeEnchantFire_;
	}
	//呪文を使っているか
	bool GetIsUseSpell();
	//呪文の属性を取得
	int32_t GetSpellType(int32_t spell);

public:
	//呪文詠唱のコヨーテ
	const float COYOTE_SPELL = 0.8f;

private:
	//マジックミサイルの間隔
	const int32_t COOLTIME_MAGIC_MISSILE = 20;
	const int32_t COOLTIME_FLAME = 5;

	float maxCharge_;
	float chargeTime_;
	float useTime_;

	std::vector<std::unique_ptr<BaseSpell>> spells_;
	std::vector<BaseSpell*> spellsList_;

	std::vector<std::unique_ptr<ChainLightning>> chainLightnings_;
	std::vector<ChainLightning*> chainLightningsList_;

	bool isModeEnchantFire_;

	float timeChargeSpell_[MAGIC_END];
	float timeFireSpell_[MAGIC_END];
	bool isUseSpell_[MAGIC_END];
};
