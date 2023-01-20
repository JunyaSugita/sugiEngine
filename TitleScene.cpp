#include "TitleScene.h"

void TitleScene::Initialize(int num)
{
	titleTexture_ = Sprite::LoadTexture("titleTex.png");
	titleSprite_.Initialize(titleTexture_);
	titleSprite_.Size(400, 200);
	titleSprite_.Pos(440, 160);

	pressTexture_ = Sprite::LoadTexture("titlePress.png");
	pressSprite_.Initialize(pressTexture_);
	pressSprite_.Size(600, 200);
	pressSprite_.Pos(340, 460);

	padTexture_ = Sprite::LoadTexture("pad.png");
	padSprite_.Initialize(padTexture_);
	padSprite_.Size(400, 200);
	padSprite_.Pos(0, 0);
}

void TitleScene::Update(Input* input)
{
	if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
		GameManager::SetStageSelectScene();
	}
}

void TitleScene::BackSpriteDraw()
{
}

void TitleScene::Draw()
{

}

void TitleScene::SpriteDraw()
{
	titleSprite_.Draw();
	pressSprite_.Draw();
	padSprite_.Draw();
}

void TitleScene::Delete()
{
}
