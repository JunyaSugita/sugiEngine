#include "sceneChange.h"
#include "GrovalSetting.h"
#include "SugiMath.h"

sceneChange::sceneChange()
{
}

sceneChange::~sceneChange()
{
}

sceneChange* sceneChange::GetInstance()
{
	static sceneChange instance;

	return &instance;
}

void sceneChange::Initialize()
{
	sceneTex_ = Sprite::LoadTexture("white1x1.png");

	sceneSp_.Initialize(sceneTex_);
	sceneSp_.SetSize(WIN_WIDTH,WIN_HEIGHT);
	sceneSp_.SetPos(pos_.x,pos_.y);
}

void sceneChange::Update()
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

void sceneChange::Draw()
{
	sceneSp_.Draw();
}
