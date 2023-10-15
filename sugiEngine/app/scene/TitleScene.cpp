#include "TitleScene.h"
#include "Input.h"
#include "ParticleManager.h"
#include "sceneChange.h"
#include "Tutorial.h"
#include "ModelManager.h"

void TitleScene::Initialize()
{
	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());

	Camera::GetInstance()->SetEye({0,-1,-10});
	Camera::GetInstance()->SetTarget({ 0,0,0 });

	ParticleManager::GetInstance()->Initialize();
	SceneChange::GetInstance()->Initialize();

	sound_.Initialize();
	sound_.LoadWave("BGM");
	if (!Tutorial::GetInstance()->GetIsReturn()) {
		sound_.PlayWave("BGM", true);
		sound_.SetVolume("BGM", 0.3f);
	}

	titleTex_ = Sprite::LoadTexture("title.png");
	titleSp_.Initialize(titleTex_);

	//武器の設定
	obj_.Initialize("weapon");
	obj_.pos = { 0,46,-48 };
	obj_.rot = { 30,0,0 };
	obj_.scale = { 1,1,1 };
	//更新
	obj_.Update();

	//オーブの設定
	orbObj_.Initialize("sphere");
	orbObj_.obj->SetColor({ 0,1,1,0.5f });
	orbObj_.obj->SetIsSimple();
	orbObj_.pos = { 0,1.7f,0 };
	orbObj_.rot = { 0,0,0 };
	orbObj_.scale = { 0.3f,0.3f,0.3f };
	orbObj_.worldTrans.parent_ = &obj_.worldTrans;
	//更新
	orbObj_.Update();
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL, {0,0.5f,0});
	ParticleManager::GetInstance()->Update();
	//ライト
	lightGroup_->Update();

	SceneChange::GetInstance()->Update();

	if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
		SceneChange::GetInstance()->Start();
	}

	if (SceneChange::GetInstance()->GetTimer() >= 1.0f) {
		ParticleManager::GetInstance()->Clear();
		GameManager::GetInstance()->SetStageSelectScene();
	}
	else if (Tutorial::GetInstance()->GetIsReturn()) {
		Tutorial::GetInstance()->SetIsReturn(false);
		GameManager::GetInstance()->SetGameScene();
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
	obj_.Draw();
}

void TitleScene::ObjDraw2()
{
	orbObj_.Draw();
}

void TitleScene::ParticleDraw()
{
	ParticleManager::GetInstance()->Draw();
}

void TitleScene::SpriteDraw()
{
	titleSp_.Draw();

	//絶対最後
	SceneChange::GetInstance()->Draw();
}

void TitleScene::Finalize()
{
	sound_.Finalize();
	ParticleManager::GetInstance()->Finalize();
}
