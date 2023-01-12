#include "StageSelectScene.h"

void StageSelectScene::Initialize(int num)
{
	tutorialTexture_ = Sprite::LoadTexture("tutorialTex.png");
	tutorialSprite_.Initialize(tutorialTexture_);
	tutorialSprite_.SetAnchorPoint(0.5f, 0.5f);
	tutorialSprite_.Size(400, 100);
	tutorialSprite_.Pos(640,100);

	easyTexture_ = Sprite::LoadTexture("easyTex.png");
	easySprite_.Initialize(easyTexture_);
	easySprite_.SetAnchorPoint(0.5f, 0.5f);
	easySprite_.Size(400, 100);
	easySprite_.Pos(640, 270);

	normalTexture_ = Sprite::LoadTexture("normalTex.png");
	normalSprite_.Initialize(normalTexture_);
	normalSprite_.SetAnchorPoint(0.5f, 0.5f);
	normalSprite_.Size(400, 100);
	normalSprite_.Pos(640, 440);

	hardTexture_ = Sprite::LoadTexture("hardTex.png");
	hardSprite_.Initialize(hardTexture_);
	hardSprite_.SetAnchorPoint(0.5f, 0.5f);
	hardSprite_.Size(400, 100);
	hardSprite_.Pos(640, 610);
}

void StageSelectScene::Update(Input* input)
{
	if (input->GetLSteckY() > 0 && change_ == false) {
		select_--;
		if (select_ < 0) {
			select_ = 0;
		}
		change_ = true;
	}
	if (input->GetLSteckY() < 0 && change_ == false) {
		select_++;
		if (select_ > 3) {
			select_ = 3;
		}
		change_ = true;
	}

	if (input->GetLSteckY() == 0) {
		change_ = false;
	}

	switch (select_)
	{
	case 0:
		tutorialSprite_.Size(440, 110);
		easySprite_.Size(400, 100);
		normalSprite_.Size(400, 100);
		hardSprite_.Size(400, 100);
		if (input->TriggerButton(XINPUT_GAMEPAD_A)) {
			GameManager::SetStage(0);
			GameManager::SetGameScene();
		}
		break;
	case 1:
		tutorialSprite_.Size(400, 100);
		easySprite_.Size(440, 110);
		normalSprite_.Size(400, 100);
		hardSprite_.Size(400, 100);
		if (input->TriggerButton(XINPUT_GAMEPAD_A)) {
			GameManager::SetStage(5);
			GameManager::SetGameScene();
		}
		break;
	case 2:
		tutorialSprite_.Size(400, 100);
		easySprite_.Size(400, 100);
		normalSprite_.Size(440, 110);
		hardSprite_.Size(400, 100);
		if (input->TriggerButton(XINPUT_GAMEPAD_A)) {
			GameManager::SetStage(10);
			GameManager::SetGameScene();
		}
		break;
	case 3:
		tutorialSprite_.Size(400, 100);
		easySprite_.Size(400, 100);
		normalSprite_.Size(400, 100);
		hardSprite_.Size(440, 110);
		if (input->TriggerButton(XINPUT_GAMEPAD_A)) {
			GameManager::SetStage(15);
			GameManager::SetGameScene();
		}
		break;
	default:
		break;
	}
}

void StageSelectScene::BackSpriteDraw()
{
}

void StageSelectScene::Draw()
{
	
}

void StageSelectScene::SpriteDraw()
{
	tutorialSprite_.Draw();
	easySprite_.Draw();
	normalSprite_.Draw();
	hardSprite_.Draw();
}

void StageSelectScene::Delete()
{
}
