#include "TitleScene.h"
#include "Input.h"
#include "ParticleManager.h"
#include "sceneChange.h"
#include "UIManager.h"

void TitleScene::Initialize()
{
	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());

	Camera::GetInstance()->SetEye({0,-1,-10});
	Camera::GetInstance()->SetTarget({ 0,0,0 });

	ParticleManager::GetInstance()->Initialize();
	UIManager::GetInstance()->Initialize();

	sound_.Initialize();
	sound_.LoadWave("Alarm01");
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL, { 5,0,0 });
	ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL, { 0,0,0 });
	ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL, { -5,0,0 });
	ParticleManager::GetInstance()->Update();
	//ライト
	lightGroup_->Update();



	if (input->TriggerKey(DIK_2) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
		SceneChange::GetInstance()->Start();
	}

	UIManager::GetInstance()->Update();

	if (SceneChange::GetInstance()->GetTimer() >= 1.0f) {
		ParticleManager::GetInstance()->Clear();
		GameManager::GetInstance()->SetGameScene();
	}

	if (input->TriggerKey(DIK_3)) {
		GameManager::GetInstance()->SetClearScene();
	}
}

void TitleScene::BackSpriteDraw()
{

}

void TitleScene::Draw()
{

}

void TitleScene::ObjDraw()
{
}

void TitleScene::ObjDraw2()
{
}

void TitleScene::ParticleDraw()
{
	ParticleManager::GetInstance()->Draw();
}

void TitleScene::SpriteDraw()
{
	SceneChange::GetInstance()->Draw();
}

void TitleScene::Finalize()
{
	sound_.Finalize();
}
