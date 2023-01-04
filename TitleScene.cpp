#include "TitleScene.h"

void TitleScene::Initialize()
{

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

}

void TitleScene::Delete()
{
}
