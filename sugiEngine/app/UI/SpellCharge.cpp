#include "SpellCharge.h"
#include "Player.h"

SpellCharge* SpellCharge::GetInstance()
{
	static SpellCharge instance;

	return &instance;
}

void SpellCharge::Initialize()
{
	spellTex_ = Sprite::LoadTexture("ChargeGauge.png");
	chargeTex_ = Sprite::LoadTexture("white1x1.png");

	spellSprite_.Initialize(spellTex_);
	spellSprite_.SetAnchorPoint(0.5f, 0.5f);
	spellSprite_.SetPos(WIN_WIDTH / 2, 500);

	chargeSprite_.Initialize(chargeTex_);
	chargeSprite_.SetAnchorPoint(0, 0.5f);
	chargeSprite_.SetPos(WIN_WIDTH / 2 - 151, 500);
	chargeSprite_.SetSize(301,50);
}

void SpellCharge::Update()
{
	if (Player::GetInstance()->GetIsSpell()) {
		chargeSprite_.SetSize(301 * SpellManager::GetInstance()->ChargePercent(), 50);
		if (SpellManager::GetInstance()->ChargePercent() >= 1.0f) {
			chargeSprite_.SetColor(1, 1, 0, 1);
		}
		else {
			chargeSprite_.SetColor(1, 1, 1, 1);
		}
	}
	if (SpellManager::GetInstance()->GetIsUseSpell()) {
		chargeSprite_.SetSize(301 * SpellManager::GetInstance()->UsePercent(), 50);
		chargeSprite_.SetColor(1, 1, 0, 1);
	}
}

void SpellCharge::Draw()
{
	if (Player::GetInstance()->GetIsSpell() || SpellManager::GetInstance()->GetIsUseSpell()) {
		chargeSprite_.Draw();
		spellSprite_.Draw();
	}
}
