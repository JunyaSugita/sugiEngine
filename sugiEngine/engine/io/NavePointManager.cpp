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
	//for (int i = 0; i < FieldManager::GetInstance()->GetNavePointNum(); i++) {
	//	if (navePoint_[i].isActive) {
	//		if (navePoint_[i].score < 15) {
	//			ParticleManager::GetInstance()->AddFromFile(P_FIRE_BALL, navePoint_[i].pos);
	//		}
	//		else if (navePoint_[i].score < 50) {
	//			ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE, navePoint_[i].pos);
	//		}
	//		else if (navePoint_[i].score != 99999) {
	//			ParticleManager::GetInstance()->AddFromFile(P_LIGHTNING, navePoint_[i].pos);
	//		}
	//	}
	//}
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

