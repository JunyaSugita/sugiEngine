#include "ClearScene.h"

void ClearScene::Initialize(int num)
{
	clearNum_ = num;

}

void ClearScene::Update(Input* input)
{
	if (input->TriggerButton(XINPUT_GAMEPAD_A)) {
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
}

void ClearScene::Delete()
{
}
