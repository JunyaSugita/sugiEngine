#include "FireBall.h"
#include "ParticleManager.h"
#include "Player.h"
#include "ModelManager.h"

void FireBall::Initialize(Vector3 pos, Vector3 vec)
{
	obj_.Initialize("sphere");
	obj_.obj->SetColor({ 1,0,0,1 });

	colObj_ = move(Object3d::Create());
	colObj_->SetModel(ModelManager::GetInstance()->Get("box"));

	obj_.rot = { 0,0,0 };
	obj_.scale = { 1,1,1 };

	vec_ = vec.normalize();

	//プレイヤーの少し前に出す
	obj_.pos = pos + vec_ * 3;

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
		obj_.pos += vec_ * SPEED_MOVE;
		if (obj_.pos.y <= 0) {
			isHit_ = true;
		}
		particleM->AddFromFile(P_FIRE_BALL, obj_.pos);
	}
	else if (isHit_) {
		Explode();
		float temp = (Player::GetInstance()->GetPos() - obj_.pos).length();
		temp = (80 - temp) / 750;
		Camera::GetInstance()->SetShake(temp);
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
	boxCol_.pos = obj_.pos;
	boxCol_.size = { obj_.scale.x,obj_.scale.y, obj_.scale.x };
}

void FireBall::WorldTransUpdate()
{
	obj_.Update();

	colWorldTrans_.SetPos(boxCol_.pos);
	colWorldTrans_.SetScale(boxCol_.size);

	SetWorldTrans();
}

void FireBall::SetWorldTrans()
{
	colObj_->SetWorldTransform(colWorldTrans_);
	colObj_->Update();
}

void FireBall::Explode()
{
	obj_.scale *= 1.2f;
	alpha_ -= 0.03f;
	obj_.obj->SetColor({ 1,0,0,alpha_ });
	ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL_EXPLODE, obj_.pos);
	if (obj_.scale.x >= 10.0f) {
		isDead_ = true;
	}
}
