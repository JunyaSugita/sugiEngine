#pragma once
#include "SugiMath.h"

struct BoxCol{
	Vector3 pos;
	float width;
	float height;
};

class ColliderManager {
private:
	ColliderManager();
	~ColliderManager();

public:
	//コピーコンストラクタ無効
	ColliderManager(const ColliderManager& obj) = delete;
	//代入演算子を無効
	ColliderManager& operator=(const ColliderManager& obj) = delete;

	static ColliderManager* GetInstance();


public:
	void Initialize();
	void Update();

	bool GetIsShowHitBox() {
		return isShowHitBox_;
	}
	void ChangeIsShowHitBox() {
		isShowHitBox_ = (isShowHitBox_ + 1) % 2;
	}

	bool ChakeHitEnemyToChainLightning();

private:
	bool ChackHitBox(BoxCol a, BoxCol b);


private:
	//当たり判定表示のフラグ
	bool isShowHitBox_;
};