#include "MagicMissile.h"
#include "ParticleManager.h"
#include "ModelManager.h"

LightGroup* MagicMissile::lightGroup_ = nullptr;

void MagicMissile::Initialize(Vector3 pos, Vector3 vec)
{
	BaseSpell::Initialize(pos,vec);

	obj_.obj->SetColor({ 1,0,1,1 });
	obj_.obj->SetEffectCross(true);
	obj_.obj->SetIsSimple();
	obj_.scale = { 0.5f,0.5f,0.5f };
	
	col_.col.pos = pos;
	col_.col.size = { 0.5f,0.5f,0.5f };

	time_ = TIME_ALIVE;
	moveSpeed_ = SPEED_MOVE;
	spellType_ = SHOT;
	damage_ = DAMAGE;
	debuffType_ = D_NONE;

	useLightNum_ = lightGroup_->SetPointLightGetNum();
	lightGroup_->SetPointLightColor(useLightNum_, { 1,0,1 });
	lightGroup_->SetPointLightAtten(useLightNum_, { 0.01f,0.01f,0.01f });
	lightGroup_->SetPointLightPos(useLightNum_, { pos.x, pos.y ,pos.z });
}

void MagicMissile::Update()
{
	if (!isHit_) {
		ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE, obj_.pos);
	}
	else {
		lightGroup_->SetPointLightActive(useLightNum_, false);
	}

	BaseSpell::Update();

	lightGroup_->SetPointLightPos(useLightNum_, { obj_.pos.x, obj_.pos.y ,obj_.pos.z });
}
