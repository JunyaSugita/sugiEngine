#include "FireBall.h"
#include "ParticleManager.h"
#include "Player.h"
#include "ModelManager.h"

void FireBall::Initialize(Vector3 pos, Vector3 vec)
{
	BaseSpell::Initialize(pos,vec);

	obj_.obj->SetColor({ 1,0,0,1 });

	alpha_ = 1.0f;

	moveSpeed_ = SPEED_MOVE;
	time_ = TIME_ALIVE;
	spellType_ = SHOT;
	damage_ = 1;
	debuffType_ = D_FIRE;
}

void FireBall::Update()
{
	if (!isHit_) {
		ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL, obj_.pos);
	}
	else {
		float temp = (Player::GetInstance()->GetPos() - obj_.pos).length();
		temp = (80 - temp) / 750;
		Camera::GetInstance()->SetShake(temp);
	}

	BaseSpell::Update();
}

void FireBall::Draw()
{
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		col_.Draw();
	}
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
