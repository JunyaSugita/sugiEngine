#include "SetSpell.h"
#include "Input.h"
#include "Player.h"
#include "UIManager.h"
#include "LoadOut.h"
#include "ColliderManager.h"

SetSpell* SetSpell::GetInstance()
{
	static SetSpell instance;

	return &instance;
}

void SetSpell::Initialize()
{
	if (!isFirst_) {
		fireBallTex_ = Sprite::LoadTexture("fireBallIcon", "png");
		magicMissileTex_ = Sprite::LoadTexture("magicMissileIcon", "png");
		iceBoltTex_ = Sprite::LoadTexture("iceBolt", "png");
		chainLightningTex_ = Sprite::LoadTexture("chainLightning", "png");
		enchantFireTex_ = Sprite::LoadTexture("enchantFireIcon", "png");
		flameTex_ = Sprite::LoadTexture("flame", "png");
		buttonTex_[0] = Sprite::LoadTexture("Button_Y", "png");
		buttonTex_[1] = Sprite::LoadTexture("Button_B", "png");
		buttonTex_[2] = Sprite::LoadTexture("Button_A", "png");
		buttonTex_[3] = Sprite::LoadTexture("Button_X", "png");

		comingSoonTex_ = Sprite::LoadTexture("comingSoonIcon", "png");
		selectHiLightTex_ = Sprite::LoadTexture("white1x1", "png");

		spellSprite_[Y_BUTTON].Initialize(fireBallTex_);
		spellSprite_[Y_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[Y_BUTTON].SetPos(POS_ORIGIN_SPELL_SPRITE.x, POS_ORIGIN_SPELL_SPRITE.y - 90);
		spellSprite_[Y_BUTTON].SetColor(1, 1, 1, 0.8f);
		spellSprite_[Y_BUTTON].SetSize(80, 80);

		spellSprite_[B_BUTTON].Initialize(magicMissileTex_);
		spellSprite_[B_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[B_BUTTON].SetPos(POS_ORIGIN_SPELL_SPRITE.x + 90, POS_ORIGIN_SPELL_SPRITE.y);
		spellSprite_[B_BUTTON].SetColor(1, 1, 1, 0.8f);
		spellSprite_[B_BUTTON].SetSize(50, 50);

		spellSprite_[A_BUTTON].Initialize(iceBoltTex_);
		spellSprite_[A_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[A_BUTTON].SetPos(POS_ORIGIN_SPELL_SPRITE.x, POS_ORIGIN_SPELL_SPRITE.y + 90);
		spellSprite_[A_BUTTON].SetColor(1, 1, 1, 0.8f);
		spellSprite_[A_BUTTON].SetSize(50, 50);

		spellSprite_[X_BUTTON].Initialize(chainLightningTex_);
		spellSprite_[X_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		spellSprite_[X_BUTTON].SetPos(POS_ORIGIN_SPELL_SPRITE.x - 90, POS_ORIGIN_SPELL_SPRITE.y);
		spellSprite_[X_BUTTON].SetColor(1, 1, 1, 0.8f);
		spellSprite_[X_BUTTON].SetSize(50, 50);

		buttonSprite_[Y_BUTTON].Initialize(buttonTex_[Y_BUTTON]);
		buttonSprite_[Y_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		buttonSprite_[Y_BUTTON].SetPos(POS_ORIGIN_SPELL_SPRITE.x, POS_ORIGIN_SPELL_SPRITE.y - 35);
		buttonSprite_[Y_BUTTON].SetColor(1, 1, 1, 0.5f);
		buttonSprite_[Y_BUTTON].SetSize(40, 40);

		buttonSprite_[B_BUTTON].Initialize(buttonTex_[B_BUTTON]);
		buttonSprite_[B_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		buttonSprite_[B_BUTTON].SetPos(POS_ORIGIN_SPELL_SPRITE.x + 35, POS_ORIGIN_SPELL_SPRITE.y);
		buttonSprite_[B_BUTTON].SetColor(1, 1, 1, 0.5f);
		buttonSprite_[B_BUTTON].SetSize(40, 40);

		buttonSprite_[A_BUTTON].Initialize(buttonTex_[A_BUTTON]);
		buttonSprite_[A_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		buttonSprite_[A_BUTTON].SetPos(POS_ORIGIN_SPELL_SPRITE.x, POS_ORIGIN_SPELL_SPRITE.y + 35);
		buttonSprite_[A_BUTTON].SetColor(1, 1, 1, 0.5f);
		buttonSprite_[A_BUTTON].SetSize(40, 40);

		buttonSprite_[X_BUTTON].Initialize(buttonTex_[X_BUTTON]);
		buttonSprite_[X_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		buttonSprite_[X_BUTTON].SetPos(POS_ORIGIN_SPELL_SPRITE.x - 35, POS_ORIGIN_SPELL_SPRITE.y);
		buttonSprite_[X_BUTTON].SetColor(1, 1, 1, 0.5f);
		buttonSprite_[X_BUTTON].SetSize(40, 40);

		weakSprite_[Y_BUTTON].Initialize(selectHiLightTex_);
		weakSprite_[Y_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		weakSprite_[Y_BUTTON].SetPos(spellSprite_[Y_BUTTON].GetPos());
		weakSprite_[Y_BUTTON].SetColor(0.8f, 0.8f, 0.f, 0.5f);
		weakSprite_[Y_BUTTON].SetSize(90, 90);

		weakSprite_[B_BUTTON].Initialize(selectHiLightTex_);
		weakSprite_[B_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		weakSprite_[B_BUTTON].SetPos(spellSprite_[B_BUTTON].GetPos());
		weakSprite_[B_BUTTON].SetColor(0.8f, 0.8f, 0.f, 0.5f);
		weakSprite_[B_BUTTON].SetSize(90, 90);

		weakSprite_[A_BUTTON].Initialize(selectHiLightTex_);
		weakSprite_[A_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		weakSprite_[A_BUTTON].SetPos(spellSprite_[A_BUTTON].GetPos());
		weakSprite_[A_BUTTON].SetColor(0.8f, 0.8f, 0.f, 0.5f);
		weakSprite_[A_BUTTON].SetSize(90, 90);

		weakSprite_[X_BUTTON].Initialize(selectHiLightTex_);
		weakSprite_[X_BUTTON].SetAnchorPoint(0.5f, 0.5f);
		weakSprite_[X_BUTTON].SetPos(spellSprite_[X_BUTTON].GetPos());
		weakSprite_[X_BUTTON].SetColor(0.8f, 0.8f, 0.f, 0.5f);
		weakSprite_[X_BUTTON].SetSize(90, 90);

		backSprite_.Initialize(selectHiLightTex_);
		backSprite_.SetAnchorPoint(0.5f,0.5f);
		backSprite_.SetPos(spellSprite_[0].GetPos());
		backSprite_.SetColor(0.1f, 0.1f, 0.1f, 0.5f);
		backSprite_.SetSize(80, 80);
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
			spellSprite_[i].SetSize(SIZE_ACTIVE_ICON);
			backSprite_.SetPos(spellSprite_[i].GetPos());
			weakSprite_[i].SetSize(85,85);
		}
		else {
			//使用中ではない呪文アイコンは小さくする
			spellSprite_[i].SetSize(SIZE_ICON);
			weakSprite_[i].SetSize(55, 55);
		}
	}

	//ロードアウト変更時にアイコン変更
	SetSpellTex();
}

void SetSpell::Draw()
{
	backSprite_.Draw();
	for (int i = 0; i < MAX_SPELL_SET; i++) {
		if (ColliderManager::GetInstance()->CheckNearEnemy(i)) {
			weakSprite_[i].Draw();
		}
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
