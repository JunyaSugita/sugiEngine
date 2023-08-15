#pragma once
#include "BaseSpell.h"
#include "FireBall.h"
#include "MagicMissile.h"
#include "IceBolt.h"
#include "ChainLightning.h"
#include "EnchantFire.h"

enum MAGIC {
	FIRE_BALL,
	MAGIC_MISSILE,
	ICE_BOLT,
	CHAIN_LIGHTNING,
	ENCHANT_FIRE,
};

class SpellManager final{
private:
	SpellManager() = default;
	~SpellManager() = default;

public:
	//�R�s�[�R���X�g���N�^����
	SpellManager(const SpellManager& obj) = delete;
	//������Z�q�𖳌�
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

	void ResetChargeTime() {
		chargeTime_ = 0;
	}
	float ChargePercent();

	float UsePercent() {
		return useTime_ / maxCharge_;
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

	bool GetActiveEnchantFire() {
		return enchantFire_->GetActive();
	}

	bool GetIsUseSpell();

public:
	//�t�@�C�A�[�{�[��
	const float TIME_CHARGE_FIREBALL = 1.0f * 60;
	const float TIME_FIRE_FIREBALL = 0.3f * 60;
	//�}�W�b�N�~�T�C��
	const float TIME_CHARGE_MAGICMISSILE = 1.0f * 60;
	const float TIME_FIRE_MAGICMISSILE = 2.0f * 60;
	//�A�C�X�{���g
	const float TIME_CHARGE_ICEBOLT = 1.2f * 60;
	const float TIME_FIRE_ICEBOLT = 0.3f * 60;
	//�`�F�C�����C�g�j���O
	const float TIME_CHARGE_CHAINLIGHTNING = 1.8f * 60;
	const float TIME_FIRE_CHAINLIGHTNING = 0.3f * 60;
	//�G���`�����g�t�@�C�A
	const float TIME_CHARGE_ENCHANTFIRE = 3.0f * 60;
	const float TIME_FIRE_ENCHANTFIRE = 0.5f * 60;

private:
	float maxCharge_;
	float chargeTime_;
	float useTime_;

	bool isUseFireBall_;
	bool isUseMagicMissile_;
	bool isUseIceBolt_;
	bool isUseChainLightning_;
	bool isUseEnchantFire_;

	std::list<std::unique_ptr<BaseSpell>> spells_;
	std::vector<BaseSpell*> spellsList_;

	std::list<std::unique_ptr<ChainLightning>> chainLightnings_;
	std::vector<ChainLightning*> chainLightningsList_;

	std::unique_ptr<EnchantFire> enchantFire_;
};