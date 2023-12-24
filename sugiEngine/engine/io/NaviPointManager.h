/**
 * @file NaviPointManager.h
 * @brief 敵からプレイヤーへの経路探索等
 */

#pragma once
#include "GlobalSetting.h"

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

	NaviPoint GetNaviPoint(int num) {
		return navePoint_[num];
	}

	void SetNaviScore(int32_t num, float score) {
		navePoint_[num].score = score;
	}

private:
	NaviPoint navePoint_[100];
};
