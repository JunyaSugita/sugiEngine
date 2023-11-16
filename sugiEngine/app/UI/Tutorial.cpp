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
	tex_[0][0] = Sprite::LoadTexture("tutorial1_1.png");
	tex_[0][1] = Sprite::LoadTexture("tutorial1_2.png");
	tex_[1][0] = Sprite::LoadTexture("tutorial2_1.png");
	tex_[1][1] = Sprite::LoadTexture("tutorial2_2.png");
	tex_[2][0] = Sprite::LoadTexture("tutorial3_1.png");
	tex_[2][1] = Sprite::LoadTexture("tutorial3_2.png");
	tex_[3][0] = Sprite::LoadTexture("tutorial4_1.png");
	tex_[3][1] = Sprite::LoadTexture("tutorial4_2.png");
	tex_[4][0] = Sprite::LoadTexture("tutorial5.png");
	tex_[4][1] = Sprite::LoadTexture("tutorial5.png");
	texStart_ = Sprite::LoadTexture("start.png");

	sprite_.Initialize(tex_[0][0]);
	sprite_.SetPos(0,0);
	sprite_.SetSize(480, 192);
	sprite_.SetColor(1,1,1,0.3f);
	sprite2_.Initialize(texStart_);
	sprite2_.SetPos(0, 200);
	sprite2_.SetSize(360, 72);
	sprite2_.SetColor(1, 1, 1, 0.3f);

	number_ = 0;
	isNext_ = false;

	animeNum_ = 0;
	animeTimer_ = ANIME_TIME;
}

void Tutorial::Update()
{
	sprite_.SetTexture(tex_[number_][animeNum_]);

	switch (number_)
	{
	case 0:
		if (Player::GetInstance()->GetPos().z >= 60) {
			isNext_ = true;
			animeNum_ = 0;
			animeTimer_ = ANIME_TIME;
		}
		break;
	case 1:
		if (Player::GetInstance()->GetPos().x >= 20) {
			isNext_ = true;
			animeNum_ = 0;
			animeTimer_ = ANIME_TIME;
		}
		break;
	case 2:
		if (Player::GetInstance()->GetPos().z >= 100) {
			isNext_ = true;
			animeNum_ = 0;
			animeTimer_ = ANIME_TIME;
		}

		break;
	case 3:
		if (Player::GetInstance()->GetPos().x <= 60) {
			isNext_ = true;
			animeNum_ = 0;
			animeTimer_ = ANIME_TIME;
		}

		break;
	default:
		break;
	}

	if (isNext_) {
		number_++;
		isNext_ = false;
	}

	if (--animeTimer_ <= 0) {
		animeTimer_ = ANIME_TIME;
		animeNum_ = (animeNum_ + 1) % 2;
	}
}

void Tutorial::Draw()
{
	if (isTutorial_) {
		sprite_.Draw();
		sprite2_.Draw();
	}
}
