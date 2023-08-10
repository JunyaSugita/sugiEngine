#include "IceBolt.h"
#include "Player.h"
#include "ParticleManager.h"
#include "ModelManager.h"

void IceBolt::Initialize(Vector3 pos, Vector3 vec)
{
	obj_.Initialize("sphere");
	obj_.obj->SetColor({ 0,0.5f,1,1 });

	col_.Initialize();


	obj_.pos = pos;
	obj_.rot = { (Player::GetInstance()->GetCameraAngle().y) * -1,Player::GetInstance()->GetCameraAngle().x,0 };
	obj_.scale = { 0.5f,0.5f,2.0f };

	vec_ = vec.normalize();

	col_.col.pos = pos;
	col_.col.size = { 0.5f ,0.5f, 0.5f };

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

	if (obj_.pos.y > 0) {
		obj_.pos += vec_ * SPEED_MOVE;
	}

	if (isHit_) {
		isDead_ = true;
	}
	else {
		ParticleManager::GetInstance()->AddFromFile(P_ICE, obj_.pos);
	}

	SetCol();

	WorldTransUpdate();
}

void IceBolt::Draw()
{
	obj_.Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		col_.Draw();
	}
}

void IceBolt::Fire()
{
	isDead_ = false;
}

void IceBolt::SetCol()
{
	col_.SetCol(obj_.pos);
	col_.col.size = { obj_.scale.x,obj_.scale.y,obj_.scale.x };
}

void IceBolt::WorldTransUpdate()
{
	obj_.Update();
	col_.Update();
}