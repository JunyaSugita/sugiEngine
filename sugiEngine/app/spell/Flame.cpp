#include "Flame.h"
#include "ParticleManager.h"
#include "Status.h"
#include "SpellManager.h"

void Flame::Initialize(const Vector3& pos, const Vector3& vec)
{
	BaseSpell::Initialize(pos, vec);

	obj_.obj->SetColor({ 1,0,0,1 });

	spellData_ = Status::GetInstance()->GetSpellData(FLAME);
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
