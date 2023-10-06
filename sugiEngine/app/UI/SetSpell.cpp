#include "SetSpell.h"
#include "Input.h"
#include "Player.h"
#include "UIManager.h"
#include "LoadOut.h"

SetSpell* SetSpell::GetInstance()
{
	static SetSpell instance;

	return &instance;
}

void SetSpell::Initialize()
{
	if (!isFirst_) {
		tex_ = Sprite::LoadTexture("back.png");
		sprite_.Initialize(tex_);
		sprite_.SetColor(1, 1, 1, 0.2f);

		fireBallTex_ = Sprite::LoadTexture("fireBallIcon.png");
		magicMissileTex_ = Sprite::LoadTexture("magicMissileIcon.png");
		iceBoltTex_ = Sprite::LoadTexture("iceBolt.png");
		chainLightningTex_ = Sprite::LoadTexture("chainLightning.png");
		enchantFireTex_ = Sprite::LoadTexture("enchantFireIcon.png");
		flameTex_ = Sprite::LoadTexture("flame.png");

		comingSoonTex_ = Sprite::LoadTexture("comingSoonIcon.png");
		selectHiLightTex_ = Sprite::LoadTexture("white1x1.png");

		spellSprite_[0].Initialize(fireBallTex_);
		spellSprite_[0].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[0].SetPos(780, 170);
		spellSprite_[0].SetColor(1, 1, 1, 0.5f);

		spellSprite_[1].Initialize(magicMissileTex_);
		spellSprite_[1].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[1].SetPos(850, 450);
		spellSprite_[1].SetColor(1, 1, 1, 0.5f);

		spellSprite_[2].Initialize(iceBoltTex_);
		spellSprite_[2].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[2].SetPos(WIN_WIDTH / 2, 600);
		spellSprite_[2].SetColor(1, 1, 1, 0.5f);

		spellSprite_[3].Initialize(chainLightningTex_);
		spellSprite_[3].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[3].SetPos(400, 450);
		spellSprite_[3].SetColor(1, 1, 1, 0.5f);

		spellSprite_[4].Initialize(enchantFireTex_);
		spellSprite_[4].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[4].SetPos(500, 170);
		spellSprite_[4].SetColor(1, 1, 1, 0.5f);

		useSpellSprite_.Initialize(fireBallTex_);
		useSpellSprite_.SetAnchorPoint(0.5f, 0.5f);
		useSpellSprite_.SetPos(1200, 635);
		useSpellSprite_.SetSize(150, 150);

		selectHiLightSp_.Initialize(selectHiLightTex_);
		selectHiLightSp_.SetAnchorPoint(0.5f, 0.5f);
		selectHiLightSp_.SetSize(110, 110);
		selectHiLightSp_.SetColor(0.5f, 0.5f, 0.5f, 0.5f);
	}
	isFirst_ = true;
}

void SetSpell::Update()
{
	LoadOut* loadOut = LoadOut::GetInstance();

	//ロードアウト変更時にアイコン変更
	SetSpellTex();

	//現在装備している魔法の右下表示の変更と変更ビューのハイライト
	useSpellSprite_.SetTexture(GetSpellTex(loadOut->GetSpell(Player::GetInstance()->GetPresetSpell())));
	selectHiLightSp_.SetPos(spellSprite_[Player::GetInstance()->GetPresetSpell()].GetPos().x, spellSprite_[Player::GetInstance()->GetPresetSpell()].GetPos().y);
}

void SetSpell::Draw()
{
	if ((Input::GetInstance()->GetLTrigger() > 50 || Input::GetInstance()->PushKey(DIK_TAB) || LoadOut::GetInstance()->GetSelectMode() == SELECT_NUM) && UIManager::GetInstance()->GetStateAlpha_() == 0 && Player::GetInstance()->GetLife() > 0) {
		sprite_.Draw();
		selectHiLightSp_.Draw();
		spellSprite_[0].Draw();
		spellSprite_[1].Draw();
		spellSprite_[2].Draw();
		spellSprite_[3].Draw();
		spellSprite_[4].Draw();
	}
	useSpellSprite_.Draw();
}

int32_t SetSpell::GetSpellTex(int32_t spellName)
{
	switch (spellName)
	{
	case FIRE_BALL:
		return fireBallTex_;
		
	case MAGIC_MISSILE:
		return magicMissileTex_;

	case ICE_BOLT:
		return iceBoltTex_;

	case CHAIN_LIGHTNING:
		return chainLightningTex_;

	case ENCHANT_FIRE:
		return enchantFireTex_;

	case FLAME:
		return flameTex_;

	default:
		break;
	}
	return 0;
}

void SetSpell::SetSpellTex()
{
	LoadOut* loadOut = LoadOut::GetInstance();

	if (loadOut->GetIsDirty()) {
		for (int i = 0; i < 5; i++) {
			spellSprite_[i].SetTexture(GetSpellTex(loadOut->GetSpell(i)));
		}
		loadOut->SetIsDirty(false);
	}
}
