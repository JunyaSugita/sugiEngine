#include "Ground.h"

std::unique_ptr<Model> Ground::sModel_ = nullptr;

void Ground::OneTimeInitialize()
{
	sModel_ = move(Model::LoadFromObj("ground"));
}

void Ground::Initialize(Vector3 pos,Vector3 scale)
{
	obj_ = move(Object3d::Create());
	obj_->SetModel(sModel_.get());

	pos_ = pos;
	rot_ = { 0,0,0 };
	scale_ = scale;

	WorldTransUpdate();
}

void Ground::Update()
{
	WorldTransUpdate();
}

void Ground::Draw()
{
	obj_->Draw();
}

void Ground::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	SetWorldTrans();
}

void Ground::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
}
