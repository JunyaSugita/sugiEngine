#pragma once
#include "Sprite.h"

class SetSpell final{
private:
	SetSpell() = default;
	~SetSpell() = default;
public:
	//コピーコンストラクタ無効
	SetSpell(const SetSpell& obj) = delete;
	//代入演算子を無効
	SetSpell& operator=(const SetSpell& obj) = delete;

	static SetSpell* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	int32_t GetSpellTex(int32_t spellName);
	void SetSpellTex();
private:
	uint32_t tex_;
	Sprite sprite_;

	uint32_t fireBallTex_;
	uint32_t magicMissileTex_;
	uint32_t iceBoltTex_;
	uint32_t chainLightningTex_;
	uint32_t enchantFireTex_;
	uint32_t flameTex_;
	uint32_t comingSoonTex_;
	uint32_t selectHiLightTex_;

	Sprite spellSprite_[5];

	Sprite useSpellSprite_;

	Sprite selectHiLightSp_;

	bool isFirst_ = false;
};