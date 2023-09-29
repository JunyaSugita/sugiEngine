#include "Tutorial.h"
#include "Input.h"
#include "SpellManager.h"
#include "Player.h"
#include "GameManager.h"
#include "StageSelectManager.h"

Tutorial* Tutorial::GetInstance()
{
	static Tutorial instance;

	return &instance;
}

void Tutorial::Initialize()
{
	tex_ = Sprite::LoadTexture("tutorial1.png");
	tex2_ = Sprite::LoadTexture("tutorial2.png");
	tex3_ = Sprite::LoadTexture("tutorial3.png");
	tex4_ = Sprite::LoadTexture("tutorial4.png");
	tex5_ = Sprite::LoadTexture("tutorial5.png");
	texStart_ = Sprite::LoadTexture("start.png");

	sprite_.Initialize(tex_);
	sprite_.SetPos(0,0);
	sprite_.SetColor(1,1,1,0.5f);
	sprite2_.Initialize(texStart_);
	sprite2_.SetPos(0, 200);
	sprite2_.SetSize(360, 72);
	sprite2_.SetColor(1, 1, 1, 0.5f);

	number_ = 0;
	isNext_ = false;

	blue_.Initialize(Sprite::LoadTexture("blueTutorial.png"));
}

void Tutorial::Update()
{
	Input* input = Input::GetInstance();

	switch (number_)
	{
	case 0:
		if (Player::GetInstance()->GetPos().z <= -20) {
			isNext_ = true;
		}
		break;
	case 1:
		sprite_.SetTexture(tex2_);
		if (Player::GetInstance()->GetPos().z <= -40) {
			isNext_ = true;
		}
		break;
	case 2:
		sprite_.SetTexture(tex3_);
		if (Player::GetInstance()->GetPos().z <= -100) {
			isNext_ = true;
		}

		break;
	case 3:
		sprite_.SetTexture(tex4_);
		if (Player::GetInstance()->GetPos().z <= -140) {
			isNext_ = true;
		}

		break;
	case 4:
		sprite_.SetTexture(tex5_);

		break;
	default:
		break;
	}

	if (isNext_) {
		number_++;
		isNext_ = false;
	}
}

void Tutorial::Draw()
{
	if (isTutorial_) {
		sprite_.Draw();
		sprite2_.Draw();
		
	}
	else {
		if (StageSelectManager::GetInstance()->GetSelectNum() != SET_SPELL_STAGE) {
			blue_.Draw();
		}
	}
}
