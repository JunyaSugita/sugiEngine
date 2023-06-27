#include "Enemy.h"

void Enemy::Initialize()
{
	model_ = move(Model::LoadFromObj("player"));
	obj_ = move(Object3d::Create());
	obj_->SetModel(model_.get());

	pos_ = {0,0,0};
	rot_ = {0,0,0};
	scale_ = {1,1,1};

	WorldTransUpdate();
}

void Enemy::Update()
{
	WorldTransUpdate();
	obj_->Update();
}

void Enemy::Draw()
{
	obj_->Draw();
}

void Enemy::WorldTransUpdate()
{
	worldTransform_.SetPos(pos_);
	worldTransform_.SetRot(rot_);
	worldTransform_.SetScale(scale_);

	SetWorldTrans();
}

void Enemy::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTransform_);
}
