#include "ClearScene.h"

void ClearScene::Initialize(int num)
{
	clearNum_ = num;

	clearTexture_ = Sprite::LoadTexture("Clear.png");
	clearSprite_.Initialize(clearTexture_);
	clearSprite_.Pos(700,300);
	clearSprite_.Size(400, 100);

	tutorialTexture_ = Sprite::LoadTexture("tutorialTex.png");
	tutorialSprite_.Initialize(tutorialTexture_);
	tutorialSprite_.Pos(300, 300);
	tutorialSprite_.Size(400, 100);

	easyTexture_ = Sprite::LoadTexture("easyTex.png");
	easySprite_.Initialize(easyTexture_);
	easySprite_.Pos(300, 300);
	easySprite_.Size(400, 100);

	normalTexture_ = Sprite::LoadTexture("normalTex.png");
	normalSprite_.Initialize(normalTexture_);
	normalSprite_.Pos(300, 300);
	normalSprite_.Size(400, 100);

	hardTexture_ = Sprite::LoadTexture("hardTex.png");
	hardSprite_.Initialize(hardTexture_);
	hardSprite_.Pos(300, 300);
	hardSprite_.Size(400, 100);

	pressTexture_ = Sprite::LoadTexture("titlePress.png");
	pressSprite_.Initialize(pressTexture_);
	pressSprite_.Size(600, 200);
	pressSprite_.Pos(340, 460);
}

void ClearScene::Update(Input* input)
{
	if (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) {
		GameManager::SetStageSelectScene();
	}
}

void ClearScene::BackSpriteDraw()
{
}

void ClearScene::Draw()
{

}

void ClearScene::SpriteDraw()
{
	clearSprite_.Draw();
	pressSprite_.Draw();
	switch (clearNum_)
	{
	case 1:
		tutorialSprite_.Draw();
		break;
	case 2:
		easySprite_.Draw();
		break;
	case 3:
		normalSprite_.Draw();
		break;
	case 4:
		hardSprite_.Draw();
		break;
	default:
		break;
	}
}

void ClearScene::Delete()
{
}
