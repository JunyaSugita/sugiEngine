#include "GameOver.h"
#include "Player.h"
#include "PostEffect.h"
#include "UIManager.h"

void GameOver::Initialize()
{
	maxHp_ = nowHp_ = Player::GetInstance()->GetLife();
	gauge_.Set({ WIN_WIDTH / 2,700 }, { 500,50 }, {0.2f,0.8f,0.2f});

	blur_ = 0;
}

void GameOver::Update()
{
	nowHp_ = Player::GetInstance()->GetLife();
	gauge_.Update((float)maxHp_, (float)nowHp_);

	if (nowHp_ <= 0) {
		if (blur_ < 5) {
			blur_ += 0.025f;
			if (blur_ >= 1) {
				PostEffect::SetBlur((int32_t)blur_);
			}
		}
		else {
			UIManager::GetInstance()->SetGameOver();
		}
	}
}

void GameOver::Draw()
{
	gauge_.Draw();
}
