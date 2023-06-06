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

	sphereWorldTransform_.pos = { 0,1,-45 };
	sphereWorldTransform_.scale = { 1,1,1 };
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	groundWorldTransform_.pos = { 0,-1,-40 };
	groundObj_->SetWorldTransform(groundWorldTransform_);
	groundObj_->Update();

	catTexture_ = Sprite::LoadTexture("cat.png");
	catSprite_.Initialize(catTexture_);
	catSprite_.SetPos(150, 520);
	catSprite_.SetSize(200, 200);

	dogTexture_ = Sprite::LoadTexture("dog.png");
	dogSprite_.Initialize(dogTexture_);
	dogSprite_.SetPos(850, 0);

	//サウンド初期化
	sound->Initialize();
	sound->LoadWave("Alarm01");

	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_);
	lightGroup_->SetCircleShadowActive(0, true);

	//sound->PlayWave("Alarm01");
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	obj1 = new Fbx;
	obj1->Initialize();
	obj1->SetModel(model1);
}

void GameScene::Update()
{

	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_LEFT)) {
		sphereWorldTransform_.pos.x -= 0.3f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		sphereWorldTransform_.pos.x += 0.3f;
	}
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	//ライト
	lightGroup_->Update();
	//丸影
	lightGroup_->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0],circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup_->SetCircleShadowCasterPos(0,
		XMFLOAT3(
			{	
				sphereWorldTransform_.pos.x,
				sphereWorldTransform_.pos.y,
				sphereWorldTransform_.pos.z
			}
	));
	lightGroup_->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup_->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	obj1->Update();

	if (input->TriggerKey(DIK_1)) {
		GameManager::GetInstance()->SetTitleScene();
	}

	Text("Hello,world %f", sphereWorldTransform_.pos.x);
	if (Button("nanimonaiyo")) {
		GameManager::GetInstance()->SetTitleScene();
	}
	SliderFloat("float", &sphereWorldTransform_.pos.x, 0.0f, 1.0f);
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

	obj1->Draw(cmdList);
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
	delete obj1;
	delete model1;
}
