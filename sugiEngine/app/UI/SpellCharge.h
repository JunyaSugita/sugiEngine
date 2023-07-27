#pragma once
#include "Sprite.h"

class SpellCharge final{
private:
	SpellCharge() = default;
	~SpellCharge() = default;
public:
	//コピーコンストラクタ無効
	SpellCharge(const SpellCharge& obj) = delete;
	//代入演算子を無効
	SpellCharge& operator=(const SpellCharge& obj) = delete;

	static SpellCharge* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

private:
	uint32_t spellTex_;
	Sprite spellSprite_;

	uint32_t chargeTex_;
	Sprite chargeSprite_;
};