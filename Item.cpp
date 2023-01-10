#include "Item.h"

void Item::Initialize()
{
	//オブジェクト
	modelItem_ = Model::LoadFromObj("item");
	objectItem_ = Object3d::Create();
	objectItem_->SetModel(modelItem_);

	InitializePos();

	SetIsUse(false);
}

void Item::Update()
{
	worldTransform_.SetWorldMat();
	objectItem_->SetWorldTransform(worldTransform_);
	objectItem_->Update();
}

void Item::Draw()
{
	objectItem_->Draw();
}

void Item::InitializePos()
{
	worldTransform_.trans = { 0,0,-500 };
	worldTransform_.scale = { 1,1,1 };
	worldTransform_.SetWorldMat();
}

void Item::SetIsUse(bool is)
{
	isUse_ = is;
	if (is == true) {
		objectItem_->SetColor({ 0.0f,0.5f,1,0.3f });
	}
	else {
		objectItem_->SetColor({ 0.0f,0.5f,1,1 });
	}
}
