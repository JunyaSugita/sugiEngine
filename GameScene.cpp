#include "GameScene.h"

void GameScene::Initialize(int num)
{
	model_ = Model::LoadFromObj("sphere");
	obj_ = Object3d::Create();
	obj_->SetModel(model_);

	worldTransform_.scale = { 10,10,10 };
	obj_->SetWorldTransform(worldTransform_);
	obj_->Update();

	catTexture_ = Sprite::LoadTexture("cat.png");
	catSprite_.Initialize(catTexture_);
	catSprite_.SetSize(300,300);
	catSprite_.SetTextureSize(100,100);
}

void GameScene::Update(Input* input)
{

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
}
