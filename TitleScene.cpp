#include "TitleScene.h"

void TitleScene::Initialize()
{
	titleTexture_ = Sprite::LoadTexture("titleTex.png");
	titleSprite_.Initialize(titleTexture_);
	titleSprite_.Size(400, 200);
}

void TitleScene::Update(Input* input)
{
	if (input->TriggerKey(DIK_SPACE)) {
		GameManager::SetGameScene();
	}
}

void TitleScene::Draw()
{

}

void TitleScene::SpriteDraw()
{
	titleSprite_.Draw();
}

void TitleScene::Delete()
{
}
