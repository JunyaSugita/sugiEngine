#include "GameScene.h"
#include "Input.h"
#include "FbxLoader.h"
#include "PostEffect.h"

using namespace ImGui;

void GameScene::Initialize()
{
	sphereModel_ = move(Model::LoadFromObj("sphere", true));
	sphereObj_ = move(Object3d::Create());
	sphereObj_->SetModel(sphereModel_.get());

	groundModel_ = move(Model::LoadFromObj("ground"));
	groundObj_ = move(Object3d::Create());
	groundObj_->SetModel(groundModel_.get());

	boxModel_ = move(Model::LoadFromObj("box"));
	boxObj_ = move(Object3d::Create());
	boxObj_->SetModel(boxModel_.get());

	sphereWorldTransform_.SetPos(Vector3());
	sphereWorldTransform_.SetScale(Vector3(1, 1, 1));
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	groundWorldTransform_.SetPos(Vector3(0, 0, 0));
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
	Object3d::SetLight(lightGroup_.get());
	Fbx::SetLight(lightGroup_.get());
	//lightGroup_->SetCircleShadowActive(0, true);

	//sound->PlayWave("Alarm01");
	model1_ = move(FbxLoader::GetInstance()->LoadModelFromFile("boneTest"));

	obj1_ = make_unique<Fbx>();
	obj1_->Initialize();
	obj1_->SetModel(model1_.get());
	obj1_->PlayAnimation();

	//カメラ
	Camera::GetInstance()->SetTarget(Vector3(0, 0, 0));
	Camera::GetInstance()->SetEye(Vector3(0, 12, -1));
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
	groundObj_->SetWorldTransform(groundWorldTransform_);
	groundObj_->Update();

	if (input->PushKey(DIK_SPACE)) {
		Camera::GetInstance()->AddEyeX(0.1f);
	}

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

	//Text("Hello,world %f", sphereWorldTransform_.GetPos().x);
	if (Button("Clear",{100,30})) {
		PostEffect::SetClear();
	}
	if (Button("Blur", { 100,30 })) {
		PostEffect::SetBlur();
	}
	if (Button("InvertColor", { 100,30 })) {
		PostEffect::SetInvertColor();
	}
	if (Button("Border", { 100,30 })) {
		PostEffect::SetBorder();
	}
	if (Button("Gray", { 100,30 })) {
		PostEffect::SetGray();
	}
	if (Button("Bloom", { 100,30 })) {
		PostEffect::SetBloom();
	}
	if (Button("Closs4", { 100,30 })) {
		PostEffect::SetCloss4();
	}
	if (Button("Closs6", { 100,30 })) {
		PostEffect::SetCloss6();
	}
	if (Button("Closs8", { 100,30 })) {
		PostEffect::SetCloss8();
	}
	//SliderFloat("float", sphereWorldTransform_.GetPosPointerX(), -5.0f, 5.0f);
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
	//sphereObj_->Draw();
	groundObj_->Draw();
	boxObj_->Draw();
}

void GameScene::SpriteDraw()
{
	//catSprite_.Draw();
}

void GameScene::Finalize()
{
	sound_->Finalize();
	model1_->Finalize();
}
