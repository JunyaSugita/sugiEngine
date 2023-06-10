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

	sphereWorldTransform_.SetPos(Vector3());
	sphereWorldTransform_.SetScale(Vector3(1, 1, 1));
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	groundWorldTransform_.SetPos(Vector3(0, -1, 0));
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
	sound_->Initialize();
	sound_->LoadWave("Alarm01");

	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_);
	lightGroup_->SetCircleShadowActive(0, true);

	//sound->PlayWave("Alarm01");
	model1_ = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	obj1_ = new Fbx;
	obj1_->Initialize();
	obj1_->SetModel(model1_);
}

void GameScene::Update()
{

	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_LEFT)) {
		sphereWorldTransform_.AddPosX(-0.3f);
	}
	if (input->PushKey(DIK_RIGHT)) {
		sphereWorldTransform_.AddPosX(0.3f);
	}
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	Camera::GetInstance()->SetTarget(Vector3(0, 20, 0));
	Camera::GetInstance()->SetEye(Vector3(0, 0, -100));

	//ライト
	lightGroup_->Update();
	//丸影
	lightGroup_->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir_[0],circleShadowDir_[1], circleShadowDir_[2], 0 }));
	lightGroup_->SetCircleShadowCasterPos(0,
		XMFLOAT3(
			{
				sphereWorldTransform_.GetPos().x,
				sphereWorldTransform_.GetPos().y,
				sphereWorldTransform_.GetPos().z
			}
	));
	lightGroup_->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten_));
	lightGroup_->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle_));

	obj1_->Update();

	if (input->TriggerKey(DIK_1)) {
		GameManager::GetInstance()->SetTitleScene();
	}

	Text("Hello,world %f", sphereWorldTransform_.GetPos().x);
	if (Button("nanimonaiyo")) {
		GameManager::GetInstance()->SetTitleScene();
	}
	SliderFloat("float", sphereWorldTransform_.GetPosPointerX(), -50.0f, 50.0f);
}

void GameScene::BackSpriteDraw()
{
	dogSprite_.Draw();
}

void GameScene::Draw()
{
	obj1_->Draw();
}

void GameScene::ObjDraw()
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
	sound_->Delete();
	delete lightGroup_;
	delete obj1_;
	delete model1_;
}
