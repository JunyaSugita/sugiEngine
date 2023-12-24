/**
 * @file ClearChecker.h
 * @brief クリア判定をし、ゴールのオブジェクトやパーティクル、UIを出す
 */

#pragma once
#include "GrovalSetting.h"
#include "Gauge.h"
#include "BaseObj.h"
#include "BaseCol.h"

class ClearChecker {
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

	void SetClear() {
		isClear_ = true;
	}
	bool GetIsClear() {
		return isClear_;
	}

	BoxCol GetCol() {
		return col_.col;
	}

	void SetGoal(Vector3 pos);

private:
	BaseObj obj_;
	BaseCol col_;

	bool isClear_;
	float blur_;
};
