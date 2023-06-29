#include "FireBall.h"
std::unique_ptr<Model> FireBall::sModel_;

void FireBall::OneTimeInitialize()
{
	sModel_ = move(Model::LoadFromObj("sphere", true));
}

void FireBall::Initialize(Vector3 pos, Vector3 vec)
{
	obj_ = move(Object3d::Create());
	obj_->SetModel(sModel_.get());
	obj_->SetColor({ 1,0,0,1 });

	pos_ = pos;
	rot_ = { 0,0,0 };
	scale_ = { 1,1,1 };

	vec_ = vec.normalize();

	WorldTransUpdate();

	isDead_ = true;
	isHit_ = false;
}

void FireBall::Update()
{
	if (isDead_) {
		return;
	}
	
	if (!isHit_) {
		pos_ += vec_ * SPEED_MOVE;
		if (pos_.y <= 0) {
			isHit_ = true;
		}
	}
	else if(isHit_){
		Explode();
	}

	

	WorldTransUpdate();
}

void FireBall::Draw()
{
	if (!isDead_) {
		obj_->Draw();
	}
}

void FireBall::Fire()
{
	isDead_ = false;
}

void FireBall::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	SetWorldTrans();
}

void FireBall::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
}

void FireBall::Explode()
{
	scale_ *= 1.2f;
	if (scale_.x >= 10.0f) {
		isDead_ = true;
	}
}
