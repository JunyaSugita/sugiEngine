#include "Bullet.h"
#include "Player.h"
#include "ColliderManager.h"

void Bullet::Initialize(Vector3 pos)
{
	//モデルデータは先に派生クラスで読み込む
	obj_.Initialize("sphere");
	obj_.pos = pos;
	BaseCol::Initialize(obj_.pos, col_.size, BULLET, gap_);

	vec_ = Player::GetInstance()->GetPos() - Vector3(pos.x,pos.y + 1,pos.z);
	vec_.normalize();

	isHit_ = false;
}

void Bullet::Update()
{
	obj_.pos += vec_ * SPEED;
	obj_.Update();

	SetCol(obj_.pos, col_.size);
	colObj_.Update();
}

void Bullet::Draw()
{
	obj_.Draw();
}

bool Bullet::GetIsHit()
{
	return isHit_;
}

void Bullet::SetIsHit()
{
	isHit_ = true;
}
