#include "GameScene.h"

void GameScene::Initialize(int num)
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

	sphereWorldTransform_.trans = { 0,1,-45 };
	sphereWorldTransform_.scale = { 1,1,1 };
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	groundWorldTransform_.trans = { 0,-1,-40 };
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
	
	imGui_ = new ImGuiManager();

	//sound->PlayWave("Alarm01");
}

void GameScene::Update(Input* input)
{
	if (input->PushKey(DIK_LEFT)) {
		sphereWorldTransform_.trans.x -= 0.3f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		sphereWorldTransform_.trans.x += 0.3f;
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
				sphereWorldTransform_.trans.x,
				sphereWorldTransform_.trans.y,
				sphereWorldTransform_.trans.z
			}
	));
	lightGroup_->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup_->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	if (input->TriggerKey(DIK_1)) {
		GameManager::SetTitleScene();
	}
	
	//ImGui
	//ImGui::Begin("abc");
	//ImGui::Text("Hello, world %d", 123);
	//ImGui::End();

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
	delete imGui_;
}
