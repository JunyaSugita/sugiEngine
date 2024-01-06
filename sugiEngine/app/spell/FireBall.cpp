#include "FireBall.h"
#include "ParticleManager.h"
#include "Player.h"
#include "ModelManager.h"

LightGroup* FireBall::lightGroup_ = nullptr;

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
	useLightNum_ = lightGroup_->SetPointLightGetNum();
	lightGroup_->SetPointLightColor(useLightNum_, { 1,0.2f,0 });
	lightGroup_->SetPointLightAtten(useLightNum_, { 0.01f,0.01f,0.01f });
	lightGroup_->SetPointLightPos(useLightNum_, { pos.x, pos.y ,pos.z });
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

	lightGroup_->SetPointLightPos(useLightNum_, { obj_.pos.x, obj_.pos.y ,obj_.pos.z });
}

void FireBall::Draw()
{

}

void FireBall::Explode()
{
	obj_.scale *= SPEED_SIZE_UP_EXPLODE;
	alpha_ -= SPEED_ALPHA_EXPLODE;
	obj_.obj->SetColor({ 1,0,0,alpha_ });
	ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL_EXPLODE, obj_.pos);
	if (obj_.scale.x >= MAX_SIZE_EXPLODE) {
		BaseSpell::Explode();
		lightGroup_->SetPointLightActive(useLightNum_,false);
	}
}
