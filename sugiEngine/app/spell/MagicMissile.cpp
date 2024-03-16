#include "MagicMissile.h"
#include "ParticleManager.h"
#include "ModelManager.h"

LightGroup* MagicMissile::lightGroup_ = nullptr;

void MagicMissile::Initialize(const Vector3& pos, const Vector3& vec)
{
	BaseSpell::Initialize(pos,vec);

	obj_.obj->SetColor({ 1,0,1,1 });
	obj_.obj->SetEffectCross(true);
	obj_.obj->SetIsSimple();
	obj_.scale = { 0.5f,0.5f,0.5f };
	
	col_.pos = pos;
	col_.size = { 0.5f,0.5f,0.5f };

	spellData_.timeAlive = TIME_ALIVE;
	spellData_.speed = SPEED_MOVE;
	spellData_.spellType = SHOT;
	spellData_.damage = DAMAGE;
	spellData_.debuffType = D_NONE;

	useLightNum_ = lightGroup_->SetPointLightGetNum();
	lightGroup_->SetPointLightColor(useLightNum_, { 1,0,1 });
	lightGroup_->SetPointLightAtten(useLightNum_, { 0.01f,0.01f,0.01f });
	lightGroup_->SetPointLightPos(useLightNum_, { pos.x, pos.y ,pos.z });
}

void MagicMissile::Update()
{
	BaseSpell::Update();

	if (!isHit_) {
		ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE, obj_.pos);
		ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE2, obj_.pos);
	}
	else {
		lightGroup_->SetPointLightActive(useLightNum_, false);
	}

	lightGroup_->SetPointLightPos(useLightNum_, { obj_.pos.x, obj_.pos.y ,obj_.pos.z });
}

void MagicMissile::Draw()
{
	BaseSpell::Draw();
}