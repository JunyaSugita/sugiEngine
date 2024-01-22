/**
 * @file SetSpell.h
 * @brief 装備した魔法を表示(UI)
 */

#pragma once
#include "Sprite.h"

enum BUTTON {
	Y_BUTTON,
	B_BUTTON,
	A_BUTTON,
	X_BUTTON,

	END_BUTTON
};

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

	//呪文のテクスチャ番号取得
	int32_t GetSpellTex(int32_t spellName);
	void SetSpellTex();
private:
	static const int32_t MAX_SPELL_SET = 4;
	const Vector2 POS_ORIGIN_SPELL_SPRITE = {1130,580};
	const Vector2 SIZE_ICON = { 50, 50 };
	const Vector2 SIZE_ACTIVE_ICON = { 80, 80 };

	uint32_t fireBallTex_;
	uint32_t magicMissileTex_;
	uint32_t iceBoltTex_;
	uint32_t chainLightningTex_;
	uint32_t enchantFireTex_;
	uint32_t flameTex_;
	uint32_t comingSoonTex_;
	uint32_t selectHiLightTex_;
	uint32_t buttonTex_[MAX_SPELL_SET];

	Sprite spellSprite_[MAX_SPELL_SET];
	Sprite buttonSprite_[MAX_SPELL_SET];
	Sprite backSprite_;
	Sprite weakSprite_[MAX_SPELL_SET];

	bool isFirst_ = false;
};
