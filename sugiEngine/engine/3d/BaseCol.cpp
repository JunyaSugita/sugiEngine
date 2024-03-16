#include "BaseCol.h"
#include "ColliderManager.h"

void BaseCol::Initialize(const Vector3& pos, const Vector3& scale, int32_t colType, float gap)
{
	colObj_.Initialize("box");

	col_.pos = pos;
	col_.size = scale;
	oldCol_ = col_;

	colType_ = colType;

	gap_ = gap;

	ColliderManager::GetInstance()->AddCollider(this);
}

void BaseCol::Update(const Vector3& pos, const Vector3& scale)
{
	SetCol(pos,scale);
	colObj_.Update();
}

void BaseCol::Draw()
{
	colObj_.Draw();
}

void BaseCol::SetCol(const Vector3& pos, const Vector3& scale)
{
	col_.pos = {pos.x,pos.y + gap_,pos.z};
	col_.size = scale;

	colObj_.pos = col_.pos;
	colObj_.scale = col_.size;
}

void BaseCol::SetOldCol()
{
	oldCol_ = col_;
}

void BaseCol::OnCollision(BaseCol* a)
{
	a;
}

void BaseCol::HitChangePos()
{

}

void BaseCol::SetIsHit()
{

}

bool BaseCol::GetIsHit()
{
	return false;
}

void BaseCol::SubLife(float subLife, bool isParticle)
{
	subLife;
	isParticle;
}

float BaseCol::GetDamage()
{
	return 0;
}

void BaseCol::SetDebuff(int32_t debuff, int32_t time)
{
	debuff;
	time;
}

int32_t BaseCol::GetDebuff()
{
	return 0;
}

void BaseCol::SetIsStop()
{
}

void BaseCol::SetKnockBack(Vector3 vec)
{
	vec;
}

float BaseCol::GetKnockBackXZ()
{
	return 0.5f;
}

float BaseCol::GetKnockBackY()
{
	return 0.0f;
}

void BaseCol::SetIsAttack()
{
}

bool BaseCol::GetIsDown()
{
	return 0;
}

void BaseCol::DownHitPlayer()
{
}

DownState BaseCol::GetDownHitEnemy()
{
	return DownState();
}

void BaseCol::SetDownHitEnemy(const DownState& state)
{
	state;
}

Vector3 BaseCol::GetPos()
{
	return Vector3();
}

void BaseCol::SetShakeTime(int32_t time)
{
	time;
}

void BaseCol::SetClear()
{
}

void BaseCol::AddColX(float num)
{
	num;
}

void BaseCol::AddColZ(float num)
{
	num;
}

int32_t BaseCol::GetSerial()
{
	return 0;
}
