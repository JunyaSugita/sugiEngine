//当たり判定を管理するマネージャークラス

#pragma once
#include "SugiMath.h"

struct BoxCol{
	Vector3 pos;
	Vector3 size;
};

class ColliderManager final{
private:
	ColliderManager() = default;
	~ColliderManager() = default;

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

	bool CheckHitEnemyToChainLightning();

	bool CanMovePlayerVec(Vector3 pos);
	int32_t CanMoveNavePointVec(Vector3 pos);

	void SetNavePointScore();

private:
	bool CheckWayX(BoxCol a, BoxCol b);
	bool CheckWayY(BoxCol a, BoxCol b);
	bool CheckWayZ(BoxCol a, BoxCol b);

	bool CheckHitX(BoxCol a, BoxCol b);
	bool CheckHitY(BoxCol a, BoxCol b);
	bool CheckHitZ(BoxCol a, BoxCol b);

	bool CheckHitBox(BoxCol a, BoxCol b);
	bool CheckHitCircle(BoxCol a, BoxCol b);

private:
	//当たり判定表示のフラグ
	bool isShowHitBox_;
};
