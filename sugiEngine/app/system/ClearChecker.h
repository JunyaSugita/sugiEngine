/**
 * @file ClearChecker.h
 * @brief クリア判定をし、ゴールのオブジェクトやパーティクル、UIを出す
 */

#pragma once
#include "GlobalSetting.h"
#include "Gauge.h"
#include "BaseObj.h"
#include "BaseCol.h"

class ClearChecker : BaseCol{
private:
	ClearChecker() = default;
	~ClearChecker() = default;

public:
	//コピーコンストラクタ無効
	ClearChecker(const ClearChecker& obj) = delete;
	//代入演算子を無効
	ClearChecker& operator=(const ClearChecker& obj) = delete;

	static ClearChecker* GetInstance();

public:
	void Initialize();
	void GameInitialize();
	void Update();
	void Draw();

	void OnCollision(BaseCol* a)override;

	//ゲッターセッター
	void SetClear() override{
		isClear_ = true;
	}
	bool GetIsClear() {
		return isClear_;
	}
	Col GetCol() {
		return col_;
	}
	void SetGoal(Vector3 pos);

private:
	//ブラー
	const float MAX_BLUR = 5;
	const float SPEED_BLUR = 0.025f;
	//ゴール
	const float GOAL_Y = 3;

	BaseObj obj_;

	bool isClear_;
	float blur_;
};
