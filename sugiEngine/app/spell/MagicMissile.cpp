#include "MagicMissile.h"
#include "ParticleManager.h"
#include "ModelManager.h"

void MagicMissile::Initialize(Vector3 pos, Vector3 vec)
{
	obj_.Initialize("sphere");
	obj_.obj->SetColor({ 1,0,1,1 });
	obj_.obj->SetEffectCross();
	obj_.obj->SetIsSimple();
	obj_.scale = { 0.5f,0.5f,0.5f };
	obj_.pos = pos + vec * 3;

	col_.Initialize();

	vec_ = vec.normalize();
	
	col_.col.pos = pos;
	col_.col.size = { 0.5f,0.5f,0.5f };

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
		obj_.pos += vec_ * SPEED_MOVE;
		ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE, obj_.pos);
		if (obj_.pos.y <= 0) {
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
	obj_.Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		col_.Draw();
	}
}

void MagicMissile::Fire()
{
	isDead_ = false;
}

void MagicMissile::SetCol()
{
	col_.SetCol(obj_.pos);
}

void MagicMissile::WorldTransUpdate()
{
	obj_.Update();
	col_.Update();
}

void MagicMissile::Explode()
{
	isDead_ = true;
}
