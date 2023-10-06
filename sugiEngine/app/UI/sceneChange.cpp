#include "SceneChange.h"
#include "GrovalSetting.h"
#include "SugiMath.h"

SceneChange* SceneChange::GetInstance()
{
	static SceneChange instance;

	return &instance;
}

void SceneChange::Initialize()
{
	sceneTex_ = Sprite::LoadTexture("white1x1.png");

	sceneSp_.Initialize(sceneTex_);
	sceneSp_.SetSize(WIN_WIDTH,WIN_HEIGHT);
	sceneSp_.SetPos(pos_.x,pos_.y);
}

void SceneChange::Update()
{
	pos_.y = -WIN_HEIGHT + EaseOut(easeTimer_, WIN_HEIGHT);

	if (isSceneChange_) {
		easeTimer_ += SPEED_TIMER;

		if (easeTimer_ >= 1.0f) {
			isSceneChange_ = false;
		}
	}
	else if (easeTimer_ > 0) {
		easeTimer_ -= SPEED_TIMER;
	}

	sceneSp_.SetPos(pos_.x, pos_.y);
}

void SceneChange::Draw()
{
	sceneSp_.Draw();
}
