#include "TitleScene.h"
#include "Input.h"
#include "ParticleManager.h"
#include "sceneChange.h"
#include "Tutorial.h"

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

	model_ = move(Model::LoadFromObj("weapon"));
	orbModel_ = move(Model::LoadFromObj("sphere", true));
	obj_ = move(Object3d::Create());
	obj_->SetModel(model_.get());
	orbObj_ = move(Object3d::Create());
	orbObj_->SetModel(orbModel_.get());
	orbObj_->SetColor({ 0,1,1,0.5f });
	orbObj_->SetIsSimple();

	pos_ = { 0,46,-48 };
	rot_ = { 30,0,0 };
	scale_ = { 1,1,1 };

	orbPos_ = { 0,1.7f,0 };
	orbRot_ = { 0,0,0 };
	orbScale_ = { 0.3f,0.3f,0.3f };

	orbTrans_.parent_ = &worldTrans_;

	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	orbTrans_.SetPos(orbPos_);
	orbTrans_.SetRot(orbRot_);
	orbTrans_.SetScale(orbScale_);

	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();

	orbObj_->SetWorldTransform(orbTrans_);
	orbObj_->Update();
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
		GameManager::GetInstance()->SetGameScene();
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
	obj_->Draw();
}

void TitleScene::ObjDraw2()
{
	orbObj_->Draw();
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
}
