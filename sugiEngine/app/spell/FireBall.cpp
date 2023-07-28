#include "FireBall.h"
#include "ParticleManager.h"
#include "Player.h"
std::unique_ptr<Model> FireBall::sModel_;
std::unique_ptr<Model> FireBall::sColModel_;

void FireBall::OneTimeInitialize()
{
	sModel_ = move(Model::LoadFromObj("sphere", true));
	sColModel_ = move(Model::LoadFromObj("box"));
}

void FireBall::Initialize(Vector3 pos, Vector3 vec)
{
	obj_ = move(Object3d::Create());
	obj_->SetModel(sModel_.get());
	obj_->SetColor({ 1,0,0,1 });

	colObj_ = move(Object3d::Create());
	colObj_->SetModel(sColModel_.get());

	rot_ = { 0,0,0 };
	scale_ = { 1,1,1 };

	vec_ = vec.normalize();

	//プレイヤーの少し前に出す
	pos_ = pos + vec_ * 3;

	boxCol_.pos = pos;
	boxCol_.size = { 1,1,1 };

	WorldTransUpdate();

	isDead_ = true;
	time_ = TIME_ALIVE;
	isHit_ = false;

	alpha_ = 1.0f;
}

void FireBall::Update()
{
	ParticleManager* particleM = ParticleManager::GetInstance();

	if (--time_ <= 0) {
		isDead_ = true;
	}

	if (!isHit_) {
		pos_ += vec_ * SPEED_MOVE;
		if (pos_.y <= 0) {
			isHit_ = true;
			float temp = (Player::GetInstance()->GetPos() - pos_).length();
			temp = (100 - temp) / 750;
			Camera::GetInstance()->SetShake(temp);
		}
		particleM->AddFromFile(P_FIRE_BALL, pos_);
	}
	else if (isHit_) {
		Explode();
	}

	SetCol();

	WorldTransUpdate();
}

void FireBall::Draw()
{
	//obj_->Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		colObj_->Draw();
	}
}

void FireBall::Fire()
{
	isDead_ = false;
}

void FireBall::SetCol()
{
	boxCol_.pos = pos_;
	boxCol_.size = { scale_.x,scale_.y,scale_.x };
}

void FireBall::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	colWorldTrans_.SetPos(boxCol_.pos);
	colWorldTrans_.SetScale(boxCol_.size);

	SetWorldTrans();
}

void FireBall::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
	colObj_->SetWorldTransform(colWorldTrans_);
	colObj_->Update();
}

void FireBall::Explode()
{
	scale_ *= 1.2f;
	alpha_ -= 0.03f;
	obj_->SetColor({ 1,0,0,alpha_ });
	ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL_EXPLODE, pos_);
	if (scale_.x >= 10.0f) {
		isDead_ = true;
	}
}
