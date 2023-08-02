#include "Tutorial.h"
#include "Input.h"
#include "SpellManager.h"
#include "Player.h"
#include "GameManager.h"

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
	sprite_.SetSize(300, 120);
	sprite_.SetPos(0,250);
	sprite2_.Initialize(texStart_);
	sprite2_.SetSize(450, 90);
	sprite2_.SetPos(0, 320);

	time_ = 60;
	number_ = 0;
	isNext_ = false;
	ease_ = 0;
}

void Tutorial::Update()
{
	Input* input = Input::GetInstance();

	sprite_.SetPos(0, 250 + sinf(ease_+= 0.01f) * 20);
	sprite2_.SetPos(-50, 380 + sinf(ease_ += 0.01f) * 20);

	switch (number_)
	{
	case 0:
		if (input->GetRStickX() != 0 || input->GetRStickY() != 0) {
			isNext_ = true;
		}
		break;
	case 1:
		sprite_.SetTexture(tex2_);
		if (input->TriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			isNext_ = true;
		}
		break;
	case 2:
		sprite_.SetTexture(tex3_);
		if (SpellManager::GetInstance()->GetIsUseSpell()) {
			isNext_ = true;
		}

		break;
	case 3:
		sprite_.SetTexture(tex4_);
		if (Player::GetInstance()->GetPresetSpell() != FIRE_BALL) {
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
		time_--;
		if (time_ < 0) {
			number_++;
			time_ = 100;
			isNext_ = false;
		}
	}
}

void Tutorial::Draw()
{
	if (isTutorial_) {
		sprite_.Draw();
		sprite2_.Draw();
	}
}
