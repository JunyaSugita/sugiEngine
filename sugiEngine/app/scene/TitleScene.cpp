#include "TitleScene.h"
#include "Input.h"
#include "ParticleManager.h"
#include "sceneChange.h"
#include "Tutorial.h"
#include "ModelManager.h"

using namespace std;

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

	ground_.Initialize("ground");
	ground_.pos = {0,0,0};
	ground_.scale = { 100.0f,0.1f,100.0f };
	ground_.obj->SetTiling({100,100});
	ground_.Update();
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	Camera* camera = Camera::GetInstance();

	if (!ParticleManager::GetInstance()->GetIsEdit()) {
		ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL, POS_FIRE);
		ParticleManager::GetInstance()->AddFromFile(P_FIRE2, POS_FIRE);
	}
	else {
		//パーティクルエディタ使用中
		if (input->PushKey(DIK_A)) {
			angle_ -= SPEED_EDIT_CAMERA_X;
		}
		if (input->PushKey(DIK_D)) {
			angle_ += SPEED_EDIT_CAMERA_X;
		}

		if (input->PushKey(DIK_R)) {
			EDIT_CAMERA_Y += SPEED_EDIT_CAMERA_Y;
		}
		if (input->PushKey(DIK_F)) {
			EDIT_CAMERA_Y -= SPEED_EDIT_CAMERA_Y;
		}

		if (input->PushKey(DIK_W)) {
			length_ -= SPEED_EDIT_CAMERA_Z;
		}
		if (input->PushKey(DIK_S)) {
			length_ += SPEED_EDIT_CAMERA_Z;
		}

		camera->SetTarget({ 0,5,0 });
		camera->SetEye({sinf(angle_) * length_,EDIT_CAMERA_Y,cosf(angle_) * length_ });
		camera->Update();

		ground_.Update();
	}
	ParticleManager::GetInstance()->Update();
	//ライト
	lightGroup_->Update();

	SceneChange::GetInstance()->Update();

	if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(XINPUT_GAMEPAD_A)) {
		SceneChange::GetInstance()->Start();
	}

	if (SceneChange::GetInstance()->GetTimer() >= 1) {
		ParticleManager::GetInstance()->Clear();
 		GameManager::GetInstance()->ChangeScene(make_unique<StageSelectScene>());
	}
	else if (Tutorial::GetInstance()->GetIsReturn()) {
		Tutorial::GetInstance()->SetIsReturn(false);
		GameManager::GetInstance()->ChangeScene(make_unique<GameScene>());
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
	if (!ParticleManager::GetInstance()->GetIsEdit()) {
		obj_.Draw();
	}
	else {
		ground_.Draw();
	}
}

void TitleScene::ObjDraw2()
{
	if (!ParticleManager::GetInstance()->GetIsEdit()) {
		orbObj_.Draw();
	}
}

void TitleScene::ParticleDraw()
{
	ParticleManager::GetInstance()->Draw();
}

void TitleScene::SpriteDraw()
{
	if (!ParticleManager::GetInstance()->GetIsEdit()) {
		titleSp_.Draw();
	}

	//絶対最後
	SceneChange::GetInstance()->Draw();
}

void TitleScene::Finalize()
{
	sound_.Finalize();
	ParticleManager::GetInstance()->Finalize();
}
