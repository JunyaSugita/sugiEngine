#include "SetSpell.h"
#include "Input.h"
#include "Player.h"

SetSpell::SetSpell()
{
}

SetSpell::~SetSpell()
{
}

SetSpell* SetSpell::GetInstance()
{
	static SetSpell instance;

	return &instance;
}

void SetSpell::Initialize()
{
	tex_ = Sprite::LoadTexture("back.png");
	sprite_.Initialize(tex_);
	sprite_.SetColor(1, 1, 1, 0.2f);

	fireBallTex_ = Sprite::LoadTexture("fireBallIcon.png");
	magicMissileTex_ = Sprite::LoadTexture("magicMissileIcon.png");
	iceBoltTex_ = Sprite::LoadTexture("iceBolt.png");
	comingSoonTex_ = Sprite::LoadTexture("comingSoonIcon.png");

	spell1Sprite_.Initialize(fireBallTex_);
	spell1Sprite_.SetAnchorPoint(0.5f, 0.5f);
	spell1Sprite_.SetPos(780, 170);
	spell1Sprite_.SetColor(1, 1, 1, 0.3f);

	spell2Sprite_.Initialize(magicMissileTex_);
	spell2Sprite_.SetAnchorPoint(0.5f, 0.5f);
	spell2Sprite_.SetPos(850, 450);
	spell2Sprite_.SetColor(1, 1, 1, 0.3f);

	spell3Sprite_.Initialize(iceBoltTex_);
	spell3Sprite_.SetAnchorPoint(0.5f, 0.5f);
	spell3Sprite_.SetPos(WIN_WIDTH / 2, 600);
	spell3Sprite_.SetColor(1, 1, 1, 0.3f);

	spell4Sprite_.Initialize(comingSoonTex_);
	spell4Sprite_.SetAnchorPoint(0.5f, 0.5f);
	spell4Sprite_.SetPos(400, 450);
	spell4Sprite_.SetColor(1, 1, 1, 0.3f);

	spell5Sprite_.Initialize(comingSoonTex_);
	spell5Sprite_.SetAnchorPoint(0.5f, 0.5f);
	spell5Sprite_.SetPos(500, 170);
	spell5Sprite_.SetColor(1, 1, 1, 0.3f);

	useSpellSprite_.Initialize(fireBallTex_);
	useSpellSprite_.SetAnchorPoint(0.5f, 0.5f);
	useSpellSprite_.SetPos(1200, 635);
	useSpellSprite_.SetSize(150, 150);

	
}

void SetSpell::Update()
{
	switch (Player::GetInstance()->GetPresetSpell())
	{
	case FIRE_BALL:
		useSpellSprite_.SetTexture(fireBallTex_);
		break;
	case MAGIC_MISSILE:
		useSpellSprite_.SetTexture(magicMissileTex_);
		break;
	case ICE:
		useSpellSprite_.SetTexture(iceBoltTex_);
		break;
	default:
		break;
	}
}

void SetSpell::Draw()
{
	if (Input::GetInstance()->GetLTrigger() > 50) {
		sprite_.Draw();
		spell1Sprite_.Draw();
		spell2Sprite_.Draw();
		spell3Sprite_.Draw();
		spell4Sprite_.Draw();
		spell5Sprite_.Draw();
	}
	useSpellSprite_.Draw();
}
