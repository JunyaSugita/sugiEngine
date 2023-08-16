#include "MagicMissile.h"
#include "ParticleManager.h"
#include "ModelManager.h"

void MagicMissile::Initialize(Vector3 pos, Vector3 vec)
{
	BaseSpell::Initialize(pos,vec);

	obj_.obj->SetColor({ 1,0,1,1 });
	obj_.obj->SetEffectCross();
	obj_.obj->SetIsSimple();
	obj_.scale = { 0.5f,0.5f,0.5f };
	
	col_.col.pos = pos;
	col_.col.size = { 0.5f,0.5f,0.5f };

	time_ = TIME_ALIVE;
	moveSpeed_ = SPEED_MOVE;
	spellType_ = SHOT;
	damage_ = 5;
	debuffType_ = D_NONE;
}

void MagicMissile::Update()
{
	if (!isHit_) {
		ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE, obj_.pos);
	}

	BaseSpell::Update();
}