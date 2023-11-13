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
		fireBallTex_ = Sprite::LoadTexture("fireBallIcon.png");
		magicMissileTex_ = Sprite::LoadTexture("magicMissileIcon.png");
		iceBoltTex_ = Sprite::LoadTexture("iceBolt.png");
		chainLightningTex_ = Sprite::LoadTexture("chainLightning.png");
		enchantFireTex_ = Sprite::LoadTexture("enchantFireIcon.png");
		flameTex_ = Sprite::LoadTexture("flame.png");
		buttonTex_[0] = Sprite::LoadTexture("Button_Y.png");
		buttonTex_[1] = Sprite::LoadTexture("Button_B.png");
		buttonTex_[2] = Sprite::LoadTexture("Button_A.png");
		buttonTex_[3] = Sprite::LoadTexture("Button_X.png");

		comingSoonTex_ = Sprite::LoadTexture("comingSoonIcon.png");
		selectHiLightTex_ = Sprite::LoadTexture("white1x1.png");

		spellSprite_[0].Initialize(fireBallTex_);
		spellSprite_[0].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[0].SetPos(POS_ORIGIN_SPELL_SPRITE.x, POS_ORIGIN_SPELL_SPRITE.y - 90);
		spellSprite_[0].SetColor(1, 1, 1, 0.5f);
		spellSprite_[0].SetSize(80, 80);

		spellSprite_[1].Initialize(magicMissileTex_);
		spellSprite_[1].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[1].SetPos(POS_ORIGIN_SPELL_SPRITE.x + 90, POS_ORIGIN_SPELL_SPRITE.y);
		spellSprite_[1].SetColor(1, 1, 1, 0.5f);
		spellSprite_[1].SetSize(50, 50);

		spellSprite_[2].Initialize(iceBoltTex_);
		spellSprite_[2].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[2].SetPos(POS_ORIGIN_SPELL_SPRITE.x, POS_ORIGIN_SPELL_SPRITE.y + 90);
		spellSprite_[2].SetColor(1, 1, 1, 0.5f);
		spellSprite_[2].SetSize(50, 50);

		spellSprite_[3].Initialize(enchantFireTex_);
		spellSprite_[3].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[3].SetPos(POS_ORIGIN_SPELL_SPRITE.x - 90, POS_ORIGIN_SPELL_SPRITE.y);
		spellSprite_[3].SetColor(1, 1, 1, 0.5f);
		spellSprite_[3].SetSize(50, 50);

		buttonSprite_[0].Initialize(buttonTex_[0]);
		buttonSprite_[0].SetAnchorPoint(0.5f, 0.5f);
		buttonSprite_[0].SetPos(POS_ORIGIN_SPELL_SPRITE.x, POS_ORIGIN_SPELL_SPRITE.y - 35);
		buttonSprite_[0].SetColor(1, 1, 1, 0.5f);
		buttonSprite_[0].SetSize(40, 40);

		buttonSprite_[1].Initialize(buttonTex_[1]);
		buttonSprite_[1].SetAnchorPoint(0.5f, 0.5f);
		buttonSprite_[1].SetPos(POS_ORIGIN_SPELL_SPRITE.x + 35, POS_ORIGIN_SPELL_SPRITE.y);
		buttonSprite_[1].SetColor(1, 1, 1, 0.5f);
		buttonSprite_[1].SetSize(40, 40);

		buttonSprite_[2].Initialize(buttonTex_[2]);
		buttonSprite_[2].SetAnchorPoint(0.5f, 0.5f);
		buttonSprite_[2].SetPos(POS_ORIGIN_SPELL_SPRITE.x, POS_ORIGIN_SPELL_SPRITE.y + 35);
		buttonSprite_[2].SetColor(1, 1, 1, 0.5f);
		buttonSprite_[2].SetSize(40, 40);

		buttonSprite_[3].Initialize(buttonTex_[3]);
		buttonSprite_[3].SetAnchorPoint(0.5f, 0.5f);
		buttonSprite_[3].SetPos(POS_ORIGIN_SPELL_SPRITE.x - 35, POS_ORIGIN_SPELL_SPRITE.y);
		buttonSprite_[3].SetColor(1, 1, 1, 0.5f);
		buttonSprite_[3].SetSize(40, 40);

		backSprite_.Initialize(selectHiLightTex_);
		backSprite_.SetAnchorPoint(0.5f,0.5f);
		backSprite_.SetPos(spellSprite_[0].GetPos());
		backSprite_.SetColor(0.1f, 0.1f, 0.1f, 0.5f);
		backSprite_.SetSize(90, 90);
	}
	isFirst_ = true;
}

void SetSpell::Update()
{
	//インスタンス取得
	Player* player = Player::GetInstance();

	//アイコンの大きさを管理
	for (int i = 0; i < MAX_SPELL_SET;i++) {
		if (player->GetPresetSpell() == i) {
			//使用中の呪文アイコンは大きくする
			spellSprite_[i].SetSize(80, 80);
			backSprite_.SetPos(spellSprite_[i].GetPos());
		}
		else {
			//使用中ではない呪文アイコンは小さくする
			spellSprite_[i].SetSize(50, 50);
		}
	}

	//ロードアウト変更時にアイコン変更
	SetSpellTex();
}

void SetSpell::Draw()
{
	backSprite_.Draw();
	for (int i = 0; i < MAX_SPELL_SET; i++) {
		
		spellSprite_[i].Draw();
		buttonSprite_[i].Draw();
	}
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
		for (int i = 0; i < MAX_SPELL_SET; i++) {
			spellSprite_[i].SetTexture(GetSpellTex(loadOut->GetSpell(i)));
		}
		loadOut->SetIsDirty(false);
	}
}
