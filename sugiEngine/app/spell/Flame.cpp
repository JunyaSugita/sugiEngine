#include "Flame.h"
#include "ParticleManager.h"

void Flame::Initialize(const Vector3& pos, const Vector3& vec)
{
	BaseSpell::Initialize(pos, vec);

	obj_.obj->SetColor({ 1,0,0,1 });

	spellData_.speed = SPEED_MOVE;
	spellData_.timeAlive = TIME_ALIVE;
	spellData_.spellType = SHOT;
	spellData_.damage = 1;
	spellData_.debuffType = D_FIRE;
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
