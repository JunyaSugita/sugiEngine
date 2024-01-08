/**
 * @file NaviPointManager.h
 * @brief 敵からプレイヤーへの経路探索等
 */

#pragma once
#include "GlobalSetting.h"
#include <vector>

struct NaviPoint {
	Vector3 pos = Vector3();
	bool isActive = false;
	float score = 99999;
};

class NaviPointManager {
private:
	NaviPointManager() = default;
	~NaviPointManager() = default;

public:
	//コピーコンストラクタ無効
	NaviPointManager(const NaviPointManager& obj) = delete;
	//代入演算子を無効
	NaviPointManager& operator=(const NaviPointManager& obj) = delete;

	static NaviPointManager* GetInstance();

public:
	void Draw();

	void Add(Vector3 pos);

	void CalcScore();

	void ReSetCalc();
	void FirstCalc();
	void SecondCalc();

private:
	const Vector3 NAVIPOINT_HITBOX = {0.5f,0.1f,0.5f};
	const float RESET_SCORE = 99999;

	std::vector<NaviPoint> naviPoints_;
};
