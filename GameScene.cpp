#include "GameScene.h"

void GameScene::Initialize(int num)
{
	model_ = Model::LoadFromObj("sphere");
	obj_ = Object3d::Create();
	obj_->SetModel(model_);

	worldTransform_.scale = { 10,10,10 };
	obj_->SetWorldTransform(worldTransform_);
	obj_->Update();
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

}

void GameScene::Delete()
{
	delete model_;
	delete obj_;
}
