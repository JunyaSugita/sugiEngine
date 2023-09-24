#include "StageSelect.h"
#include "SceneChange.h"
#include "StageSelectManager.h"
#include "Input.h"

void StageSelect::Initialize()
{
	StageSelectManager::GetInstance()->Initialize();
}

void StageSelect::Update()
{
	//シーンチェンジ
	SceneChange::GetInstance()->Update();

	StageSelectManager::GetInstance()->Update();

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Start();
	}
	if (SceneChange::GetInstance()->GetTimer() >= 1.0f) {
		GameManager::GetInstance()->SetGameScene();
	}
}

void StageSelect::BackSpriteDraw()
{
}

void StageSelect::Draw()
{
	
}

void StageSelect::ObjDraw()
{
}

void StageSelect::ObjDraw2()
{
}

void StageSelect::ParticleDraw()
{
}

void StageSelect::SpriteDraw()
{
	StageSelectManager::GetInstance()->Draw();
	SceneChange::GetInstance()->Draw();
}

void StageSelect::Finalize()
{
}
