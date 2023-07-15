#include "IceBolt.h"
#include "Player.h"
#include "ParticleManager.h"
std::unique_ptr<Model> IceBolt::sModel_;
std::unique_ptr<Model> IceBolt::sColModel_;

void IceBolt::OneTimeInitialize()
{
	sModel_ = move(Model::LoadFromObj("sphere", true));
	sColModel_ = move(Model::LoadFromObj("box"));
}

void IceBolt::Initialize(Vector3 pos, Vector3 vec)
{
	obj_ = move(Object3d::Create());
	obj_->SetModel(sModel_.get());
	obj_->SetColor({ 0,0.5f,1,1 });

	colObj_ = move(Object3d::Create());
	colObj_->SetModel(sColModel_.get());


	pos_ = pos;
	rot_ = { (Player::GetInstance()->GetCameraAngle().y) * -1,Player::GetInstance()->GetCameraAngle().x,0 };
	scale_ = { 0.5f,0.5f,2.0f };

	vec_ = vec.normalize();

	boxCol_.pos = pos;
	boxCol_.width = 0.5f;
	boxCol_.height = 0.5f;

	WorldTransUpdate();

	isDead_ = true;
	time_ = TIME_ALIVE;
	isHit_ = false;

	alpha_ = 1.0f;
}

void IceBolt::Update()
{
	if (--time_ <= 0) {
		isDead_ = true;
	}

	if (pos_.y > 0) {
		pos_ += vec_ * SPEED_MOVE;
	}

	if (isHit_) {
		isDead_ = true;
	}
	else {
		ParticleManager::GetInstance()->AddFromFile(P_ICE,pos_);
	}

	SetCol();

	WorldTransUpdate();
}

void IceBolt::Draw()
{
	obj_->Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		colObj_->Draw();
	}
}

void IceBolt::Fire()
{
	isDead_ = false;
}

void IceBolt::SetCol()
{
	boxCol_.pos = pos_;
	boxCol_.width = scale_.x;
	boxCol_.height = scale_.y;
}

void IceBolt::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	colWorldTrans_.SetPos(boxCol_.pos);
	colWorldTrans_.SetScale({ boxCol_.width,boxCol_.height,boxCol_.width });

	SetWorldTrans();
}

void IceBolt::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
	colObj_->SetWorldTransform(colWorldTrans_);
	colObj_->Update();
}
