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
	tex_[TUTORIAL0][ANIME0] = Sprite::LoadTexture("tutorial1_1", "png");
	tex_[TUTORIAL0][ANIME1] = Sprite::LoadTexture("tutorial1_2", "png");
	tex_[TUTORIAL1][ANIME0] = Sprite::LoadTexture("tutorial2_1", "png");
	tex_[TUTORIAL1][ANIME1] = Sprite::LoadTexture("tutorial2_2", "png");
	tex_[TUTORIAL2][ANIME0] = Sprite::LoadTexture("tutorial3_1", "png");
	tex_[TUTORIAL2][ANIME1] = Sprite::LoadTexture("tutorial3_2", "png");
	tex_[TUTORIAL3][ANIME0] = Sprite::LoadTexture("tutorial4_1", "png");
	tex_[TUTORIAL3][ANIME1] = Sprite::LoadTexture("tutorial4_2", "png");
	tex_[TUTORIAL4][ANIME0] = Sprite::LoadTexture("tutorial5", "png");
	tex_[TUTORIAL4][ANIME1] = Sprite::LoadTexture("tutorial5", "png");
	texStart_ = Sprite::LoadTexture("start", "png");

	sprite_.Initialize(tex_[TUTORIAL0][ANIME0]);
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
	case TUTORIAL0:
		if (Player::GetInstance()->GetPos().z >= TUTORIAL_0TO1) {
			isNext_ = true;
			animeNum_ = ANIME0;
			animeTimer_ = ANIME_TIME;
		}
		break;
	case TUTORIAL1:
		if (Player::GetInstance()->GetPos().x >= TUTORIAL_1TO2) {
			isNext_ = true;
			animeNum_ = ANIME0;
			animeTimer_ = ANIME_TIME;
		}
		break;
	case TUTORIAL2:
		if (Player::GetInstance()->GetPos().z >= TUTORIAL_2TO3) {
			isNext_ = true;
			animeNum_ = ANIME0;
			animeTimer_ = ANIME_TIME;
		}

		break;
	case TUTORIAL3:
		if (Player::GetInstance()->GetPos().x <= TUTORIAL_3TO4) {
			isNext_ = true;
			animeNum_ = ANIME0;
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
