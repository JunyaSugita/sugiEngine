#include "ChainLightning.h"
std::unique_ptr<Model> ChainLightning::sModel_;
std::unique_ptr<Model> ChainLightning::sColModel_;

void ChainLightning::OneTimeInitialize()
{
	sModel_ = move(Model::LoadFromObj("sphere", true));
	sColModel_ = move(Model::LoadFromObj("box"));
}

void ChainLightning::Initialize(Vector3 pos, Vector3 vec)
{
	obj_ = move(Object3d::Create());
	obj_->SetModel(sModel_.get());
	obj_->SetColor({ 1,0,0,1 });

	colObj_ = move(Object3d::Create());
	colObj_->SetModel(sColModel_.get());


	pos_ = pos;
	rot_ = { 0,0,0 };
	scale_ = { 1,1,1 };

	vec_ = vec.normalize();

	boxCol_.pos = pos;
	boxCol_.width = 1;
	boxCol_.height = 1;

	WorldTransUpdate();

	isDead_ = true;
	time_ = TIME_ALIVE;
	isHit_ = false;

	alpha_ = 1.0f;
}

void ChainLightning::Update()
{
	if (--time_ <= 0) {
		isDead_ = true;
	}

	if (!isHit_) {
		pos_ += vec_ * SPEED_MOVE;
		if (pos_.y <= 0) {
			isHit_ = true;
		}
	}
	else if (isHit_) {
		Explode();
	}

	SetCol();

	WorldTransUpdate();
}

void ChainLightning::Draw()
{
	obj_->Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		colObj_->Draw();
	}
}

void ChainLightning::Fire()
{
	isDead_ = false;
}

void ChainLightning::SetCol()
{
	boxCol_.pos = pos_;
	boxCol_.width = scale_.x;
	boxCol_.height = scale_.y;
}

void ChainLightning::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	colWorldTrans_.SetPos(boxCol_.pos);
	colWorldTrans_.SetScale({ boxCol_.width,boxCol_.height,boxCol_.width });

	SetWorldTrans();
}

void ChainLightning::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
	colObj_->SetWorldTransform(colWorldTrans_);
	colObj_->Update();
}

void ChainLightning::Explode()
{
	scale_ *= 1.2f;
	alpha_ -= 0.03f;
	obj_->SetColor({ 1,0,0,alpha_ });
	if (scale_.x >= 10.0f) {
		isDead_ = true;
	}
}
