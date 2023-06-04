#include "GameScene.h"
#include "Input.h"
#include "FbxLoader.h"

using namespace ImGui;

void GameScene::Initialize()
{
	sphereModel_ = Model::LoadFromObj("sphere", true);
	sphereObj_ = Object3d::Create();
	sphereObj_->SetModel(sphereModel_);

	groundModel_ = Model::LoadFromObj("ground");
	groundObj_ = Object3d::Create();
	groundObj_->SetModel(groundModel_);

	boxModel_ = Model::LoadFromObj("box");
	boxObj_ = Object3d::Create();
	boxObj_->SetModel(boxModel_);

	sphereWorldTransform_.trans_ = { 0,1,-45 };
	sphereWorldTransform_.scale_ = { 1,1,1 };
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	groundWorldTransform_.trans_ = { 0,-1,-40 };
	groundObj_->SetWorldTransform(groundWorldTransform_);
	groundObj_->Update();

	catTexture_ = Sprite::LoadTexture("cat.png");
	catSprite_.Initialize(catTexture_);
	catSprite_.SetPos(150, 520);
	catSprite_.SetSize(200, 200);

	dogTexture_ = Sprite::LoadTexture("dog.png");
	dogSprite_.Initialize(dogTexture_);
	dogSprite_.SetPos(850, 0);

	//�T�E���h������
	sound->Initialize();
	sound->LoadWave("Alarm01");

	//���C�g
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_);
	lightGroup_->SetCircleShadowActive(0, true);

	//sound->PlayWave("Alarm01");
	FbxLoader::GetInstance()->LoadModelFromFile("cube");
}

void GameScene::Update()
{

	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_LEFT)) {
		sphereWorldTransform_.trans_.x -= 0.3f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		sphereWorldTransform_.trans_.x += 0.3f;
	}
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	//���C�g
	lightGroup_->Update();
	//�ۉe
	lightGroup_->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0],circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup_->SetCircleShadowCasterPos(0,
		XMFLOAT3(
			{	
				sphereWorldTransform_.trans_.x,
				sphereWorldTransform_.trans_.y,
				sphereWorldTransform_.trans_.z
			}
	));
	lightGroup_->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup_->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	if (input->TriggerKey(DIK_1)) {
		GameManager::GetInstance()->SetTitleScene();
	}

	Text("Hello,world %f", sphereWorldTransform_.trans_.x);
	if (Button("nanimonaiyo")) {
		GameManager::GetInstance()->SetTitleScene();
	}
	SliderFloat("float", &sphereWorldTransform_.trans_.x, 0.0f, 1.0f);
}

void GameScene::BackSpriteDraw()
{
	dogSprite_.Draw();
}

void GameScene::Draw()
{
	sphereObj_->Draw();
	groundObj_->Draw();
	boxObj_->Draw();
}

void GameScene::SpriteDraw()
{
	catSprite_.Draw();
	
}

void GameScene::Delete()
{
	delete sphereModel_;
	delete sphereObj_;
	delete groundModel_;
	delete groundObj_;
	sound->Delete();
	delete lightGroup_;
}