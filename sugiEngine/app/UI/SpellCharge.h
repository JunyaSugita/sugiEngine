#pragma once
#include "Sprite.h"

class SpellCharge final{
private:
	SpellCharge() = default;
	~SpellCharge() = default;
public:
	//�R�s�[�R���X�g���N�^����
	SpellCharge(const SpellCharge& obj) = delete;
	//������Z�q�𖳌�
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