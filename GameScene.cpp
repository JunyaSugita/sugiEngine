#include "GameScene.h"

void GameScene::Initialize(int num)
{
	model_ = Model::LoadFromObj("sphere",true);
	obj_ = Object3d::Create();
	obj_->SetModel(model_);

	worldTransform_.scale = { 10,10,10 };
	obj_->SetWorldTransform(worldTransform_);
	obj_->Update();

	catTexture_ = Sprite::LoadTexture("cat.png");
	catSprite_.Initialize(catTexture_);
	catSprite_.SetSize(300,300);
	catSprite_.SetTextureSize(100,100);

	//サウンド初期化
	sound->Initialize();
	sound->LoadWave("Alarm01");

	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_);


	sound->PlayWave("Alarm01");
}

void GameScene::Update(Input* input)
{
	worldTransform_.rotation.z += 0.3f;
	obj_->SetWorldTransform(worldTransform_);
	obj_->Update();

	//ライト
	lightGroup_->Update();
}

void GameScene::BackSpriteDraw()
{

}

void GameScene::Draw()
{
	obj_->Draw();
}

void GameScene::SpriteDraw()
{
	catSprite_.Draw();
}

void GameScene::Delete()
{
	delete model_;
	delete obj_;
	sound->Delete();
	delete lightGroup_;
}
