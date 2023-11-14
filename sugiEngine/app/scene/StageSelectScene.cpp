#include "StageSelectScene.h"
#include "SceneChange.h"
#include "StageSelectManager.h"
#include "Input.h"

void StageSelectScene::Initialize()
{
	StageSelectManager::GetInstance()->Initialize();
}

void StageSelectScene::Update()
{
	//シーンチェンジ
	SceneChange::GetInstance()->Update();

	StageSelectManager::GetInstance()->Update();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_A)) {
		SceneChange::GetInstance()->Start();
	}
	if (SceneChange::GetInstance()->GetTimer() >= 1.0f) {
		GameManager::GetInstance()->SetGameScene();
	}
}

void StageSelectScene::BackSpriteDraw()
{
}

void StageSelectScene::Draw()
{
}

void StageSelectScene::ObjDraw()
{
}

void StageSelectScene::ObjDraw2()
{
}

void StageSelectScene::ParticleDraw()
{
}

void StageSelectScene::SpriteDraw()
{
	StageSelectManager::GetInstance()->Draw();
	SceneChange::GetInstance()->Draw();
}

void StageSelectScene::Finalize()
{
	
}
