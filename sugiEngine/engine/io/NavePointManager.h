#pragma once
#include "GrovalSetting.h"

struct NavePoint {
	Vector3 pos = Vector3();
	bool isActive = false;
	int32_t score = 99;
};

class NavePointManager {
private:
	NavePointManager() = default;
	~NavePointManager() = default;

public:
	//コピーコンストラクタ無効
	NavePointManager(const NavePointManager& obj) = delete;
	//代入演算子を無効
	NavePointManager& operator=(const NavePointManager& obj) = delete;

	static NavePointManager* GetInstance();

public:
	void Draw();

	void Add(Vector3 pos);

	NavePoint GetNavePoint(int num) {
		return navePoint_[num];
	}

	void SetNaveScore(int32_t num, int32_t score) {
		navePoint_[num].score = score;
	}

private:
	NavePoint navePoint_[100];
};