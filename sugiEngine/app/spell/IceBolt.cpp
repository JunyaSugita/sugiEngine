#include "IceBolt.h"
#include "Player.h"
#include "ParticleManager.h"
#include "ModelManager.h"

void IceBolt::Initialize(Vector3 pos, Vector3 vec)
{
	BaseSpell::Initialize(pos,vec);
	obj_.obj->SetColor({ 0,0.5f,1,1 });

	obj_.pos = pos;
	obj_.rot = { (Player::GetInstance()->GetCameraAngle().y) * -1,Player::GetInstance()->GetCameraAngle().x,0 };
	obj_.scale = { 0.5f,0.5f,2.0f };

	vec_ = vec.normalize();

	col_.col.pos = pos;
	col_.col.size = { 0.5f ,0.5f, 0.5f };

	WorldTransUpdate();

	moveSpeed_ = SPEED_MOVE;
	time_ = TIME_ALIVE;
	spellType_ = SHOT;
	damage_ = 15;
	debuffType_ = D_SLOW;
}

void IceBolt::Update()
{
	if (!isHit_) {
		ParticleManager::GetInstance()->AddFromFile(P_ICE, obj_.pos);
	}

	BaseSpell::Update();
}