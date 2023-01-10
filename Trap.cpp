#include "Trap.h"

void Trap::Initialize()
{
	//オブジェクト
	modelTrap_ = Model::LoadFromObj("trap");
	objectTrap_ = Object3d::Create();
	objectTrap_->SetModel(modelTrap_);

	InitializePos();

	objectTrap_->SetColor({ 0.1f,0.1f,0.1f,1 });
}

void Trap::Update()
{

	objectTrap_->SetWorldTransform(worldTransform_);
	objectTrap_->Update();
}

void Trap::Draw()
{
	objectTrap_->Draw();
}

void Trap::InitializePos()
{
	worldTransform_.trans = { 0,0,-500 };
	worldTransform_.scale = { 1,1,1 };
	worldTransform_.SetWorldMat();
}
