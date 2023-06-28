#include "Graund.h"

std::unique_ptr<Model> Graund::sModel_ = nullptr;

void Graund::OneTimeInitialize()
{
	sModel_ = move(Model::LoadFromObj("ground"));
}

void Graund::Initialize(Vector3 pos,Vector3 scale)
{
	obj_ = move(Object3d::Create());
	obj_->SetModel(sModel_.get());

	pos_ = pos;
	rot_ = { 0,0,0 };
	scale_ = scale;

	WorldTransUpdate();
}

void Graund::Update()
{
	WorldTransUpdate();
}

void Graund::Draw()
{
	obj_->Draw();
}

void Graund::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	SetWorldTrans();
}

void Graund::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
}
