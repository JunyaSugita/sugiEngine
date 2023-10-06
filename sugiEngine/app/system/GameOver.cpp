#include "GameOver.h"
#include "Player.h"
#include "PostEffectSecond.h"
#include "UIManager.h"

void GameOver::Initialize()
{
	maxHp_ = nowHp_ = Player::GetInstance()->GetLife();
	gauge_.Set({ WIN_WIDTH / 2,700 }, { 500,50 }, {0.2f,0.8f,0.2f});

	blur_ = 0;
	PostEffectSecond::SetBlur((int32_t)blur_);
}

void GameOver::Update()
{
	nowHp_ = Player::GetInstance()->GetLife();
	gauge_.Update((float)maxHp_, (float)nowHp_,(float)Player::GetInstance()->GetHeal());

	if (nowHp_ <= 0) {
		if (blur_ < 5) {
			blur_ += 0.025f;
			if (blur_ >= 1) {
				PostEffectSecond::SetBlur((int32_t)blur_);
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
