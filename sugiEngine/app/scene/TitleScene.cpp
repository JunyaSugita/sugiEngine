#include "TitleScene.h"
#include "Input.h"
#include "ParticleManager.h"

void TitleScene::Initialize()
{
	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());

	Camera::GetInstance()->SetEye({0,-1,-10});
	Camera::GetInstance()->SetTarget({ 0,0,0 });

	ParticleManager::GetInstance()->Initialize();
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

	if (input->PushKey(DIK_2) || input->PushButton(XINPUT_GAMEPAD_A)) {
		ParticleManager::GetInstance()->Clear();
		
	}
	else if (input->ReleaseKey(DIK_2) || input->ReleaseButton(XINPUT_GAMEPAD_A)) {
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

void TitleScene::ParticleDraw()
{
	ParticleManager::GetInstance()->Draw();
}

void TitleScene::SpriteDraw()
{

}

void TitleScene::Finalize()
{
}
