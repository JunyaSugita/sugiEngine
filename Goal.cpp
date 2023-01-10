#include "Goal.h"

void Goal::Initialize()
{
	//オブジェクト
	modelGoal_ = Model::LoadFromObj("goal");
	objectGoal_ = Object3d::Create();
	objectGoal_->SetModel(modelGoal_);

	worldTransform_.trans = { 0,0,-500 };
	worldTransform_.scale = { 2,2,2 };
	worldTransform_.rotation = { 0,180,45 };
	worldTransform_.SetWorldMat();

	objectGoal_->SetColor({ 1,1,1,1 });
}

void Goal::Update()
{
	objectGoal_->SetWorldTransform(worldTransform_);
	objectGoal_->Update();
}

void Goal::Draw()
{
	objectGoal_->Draw();
}
