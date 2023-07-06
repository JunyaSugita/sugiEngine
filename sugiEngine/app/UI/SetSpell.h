#pragma once
#include "Sprite.h"

class SetSpell {
private:
	SetSpell();
	~SetSpell();
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

private:
	uint32_t tex_;
	Sprite sprite_;

	uint32_t fireBallTex_;
	uint32_t magicMissileTex_;
	uint32_t iceBoltTex_;
	uint32_t chainLightningTex_;
	uint32_t comingSoonTex_;

	Sprite spell1Sprite_;
	Sprite spell2Sprite_;
	Sprite spell3Sprite_;
	Sprite spell4Sprite_;
	Sprite spell5Sprite_;

	Sprite useSpellSprite_;
};