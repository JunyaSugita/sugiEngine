/**
 * @file SpellCharge.h
 * @brief 魔法を溜めているときに出てくるゲージの描画(UI)
 */

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
	const Vector2 SIZE_CHARGE_GAUGE = { 301 ,50 };
	const Vector4 COLOR_GAUGE = { 1, 1, 0, 1 };
	const Vector4 COLOR_MAX_GAUGE = { 1, 1, 1, 1 };

	uint32_t spellTex_;
	Sprite spellSprite_;

	uint32_t chargeTex_;
	Sprite chargeSprite_;
};
