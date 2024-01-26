#include "Flame.h"
#include "ParticleManager.h"

void Flame::Initialize(Vector3 pos, Vector3 vec)
{
	BaseSpell::Initialize(pos, vec);

	obj_.obj->SetColor({ 1,0,0,1 });

	moveSpeed_ = SPEED_MOVE;
	time_ = TIME_ALIVE;
	spellType_ = SHOT;
	damage_ = 1;
	debuffType_ = D_FIRE;
}

void Flame::Update()
{
	if (!isHit_) {
		ParticleManager::GetInstance()->AddFromFile(P_FIRE, obj_.pos);
	}

	BaseSpell::Update();
}

void Flame::Draw()
{

}
