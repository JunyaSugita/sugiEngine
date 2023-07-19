#include "MagicMissile.h"
#include "ParticleManager.h"
std::unique_ptr<Model> MagicMissile::sModel_;
std::unique_ptr<Model> MagicMissile::sColModel_;

void MagicMissile::OneTimeInitialize()
{
	sModel_ = move(Model::LoadFromObj("sphere", true));
	sColModel_ = move(Model::LoadFromObj("box"));
}

void MagicMissile::Initialize(Vector3 pos, Vector3 vec)
{
	obj_ = move(Object3d::Create());
	obj_->SetModel(sModel_.get());
	obj_->SetColor({ 1,0,1,1 });

	colObj_ = move(Object3d::Create());
	colObj_->SetModel(sColModel_.get());

	pos_ = pos;
	rot_ = { 0,0,0 };
	scale_ = { 0.5f,0.5f,0.5f };

	vec_ = vec.normalize();

	boxCol_.pos = pos;
	boxCol_.size = { 0.1f,0.1f,0.1f };

	WorldTransUpdate();

	isDead_ = true;
	time_ = TIME_ALIVE;
	isHit_ = false;
}

void MagicMissile::Update()
{
	if (time_ <= 0) {
		isDead_ = true;
	}

	if (!isHit_) {
		pos_ += vec_ * SPEED_MOVE;
		ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE,pos_);
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

void MagicMissile::Draw()
{
	obj_->Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		colObj_->Draw();
	}
}

void MagicMissile::Fire()
{
	isDead_ = false;
}

void MagicMissile::SetCol()
{
	boxCol_.pos = pos_;
}

void MagicMissile::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	colWorldTrans_.SetPos(boxCol_.pos);
	colWorldTrans_.SetScale(boxCol_.size);

	SetWorldTrans();
}

void MagicMissile::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
	colObj_->SetWorldTransform(colWorldTrans_);
	colObj_->Update();
}

void MagicMissile::Explode()
{
	isDead_ = true;
}
