#include "NaviPointManager.h"
#include "ParticleManager.h"
#include "Player.h"
#include "ColliderManager.h"
#include "ImGuiManager.h"

using namespace ImGui;

NaviPointManager* NaviPointManager::GetInstance()
{
	static NaviPointManager instance;

	return &instance;
}

void NaviPointManager::Initialize()
{
	naviPoints_.clear();
}

void NaviPointManager::Draw()
{
	for (int i = 0; i < naviPoints_.size(); i++) {
		if (naviPoints_[i].score < 99999) {
			ParticleManager::GetInstance()->AddFromFile(P_FIRE, naviPoints_[i].pos);
		}
		else if (naviPoints_[i].score == 99999) {
			ParticleManager::GetInstance()->AddFromFile(P_MAGIC_MISSILE, naviPoints_[i].pos);
		}
	}
}

void NaviPointManager::Add(Vector3 pos)
{
	NaviPoint temp;

	temp.isActive = true;
	temp.pos = pos;

	naviPoints_.push_back(temp);
}

void NaviPointManager::CalcScore()
{
	ReSetCalc();

	FirstCalc();

	SecondCalc();

#ifdef _DEBUG
	Begin("NaviPoint");
	for (int i = 0; i < naviPoints_.size(); i++) {
		Text("%d,%f", i, naviPoints_[i].score);
	}
	End();
#endif // _DEBUG
}

void NaviPointManager::ReSetCalc()
{
	for (int i = 0; i < naviPoints_.size(); i++) {
		naviPoints_[i].score = RESET_SCORE;
	}
}

void NaviPointManager::FirstCalc()
{
	for (int i = 0; i < naviPoints_.size(); i++) {
		if (ColliderManager::GetInstance()->CanMoveToPlayer(naviPoints_[i].pos)) {
			float temp = (Player::GetInstance()->GetPos() - naviPoints_[i].pos).length();

			naviPoints_[i].score = temp;
		}
	}
}

void NaviPointManager::SecondCalc()
{
	for (int k = 0; k < 5; k++) {
		for (int i = 0; i < naviPoints_.size(); i++) {
			if (naviPoints_[i].score == RESET_SCORE) {
				for (int j = 0; j < naviPoints_.size(); j++) {
					if (naviPoints_[j].score != RESET_SCORE) {
						if (ColliderManager::GetInstance()->CanMoveToNaviPoint(naviPoints_[i].pos, naviPoints_[j].pos)) {
							float temp = (Player::GetInstance()->GetPos() - naviPoints_[i].pos).length();

							naviPoints_[i].score = temp + naviPoints_[j].score;
						}
					}
				}
			}
		}
	}
}

