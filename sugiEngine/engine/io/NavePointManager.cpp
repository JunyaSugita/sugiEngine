#include "NavePointManager.h"
#include "ParticleManager.h"
#include "FieldManager.h"

NavePointManager* NavePointManager::GetInstance()
{
	static NavePointManager instance;

	return &instance;
}

void NavePointManager::Draw()
{
	for (int i = 0; i < FieldManager::GetInstance()->GetNavePointNum(); i++) {
		if (navePoint_[i].score == 1) {
			ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL, navePoint_[i].pos);
		}
		else if (navePoint_[i].score == 2) {
			ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE, navePoint_[i].pos);
		}
		else if (navePoint_[i].score == 3) {
			ParticleManager::GetInstance()->AddFromFile(P_LIGHTNING, navePoint_[i].pos);
		}
	}
}

void NavePointManager::Add(Vector3 pos)
{
	for (int i = 0; i < 100; i++) {
		if (!navePoint_[i].isActive) {
			navePoint_[i].isActive = true;
			navePoint_[i].pos = pos;
			return;
		}
	}
}

