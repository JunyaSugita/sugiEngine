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
		chargeSprite_.SetSize(SIZE_CHARGE_GAUGE.x * SpellManager::GetInstance()->ChargePercent(), SIZE_CHARGE_GAUGE.y);
		if (SpellManager::GetInstance()->ChargePercent() >= 1.0f) {
			chargeSprite_.SetColor(COLOR_GAUGE);
		}
		else {
			chargeSprite_.SetColor(COLOR_MAX_GAUGE);
		}
	}
	if (SpellManager::GetInstance()->GetIsUseSpell()) {
		chargeSprite_.SetSize(SIZE_CHARGE_GAUGE.x * SpellManager::GetInstance()->UsePercent(), SIZE_CHARGE_GAUGE.y);
		chargeSprite_.SetColor(COLOR_GAUGE);
	}
}

void SpellCharge::Draw()
{
	if (Player::GetInstance()->GetIsSpell() || SpellManager::GetInstance()->GetIsUseSpell()) {
		chargeSprite_.Draw();
		spellSprite_.Draw();
	}
}
