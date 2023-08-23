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
	//�R�s�[�R���X�g���N�^����
	ClearChecker(const ClearChecker& obj) = delete;
	//������Z�q�𖳌�
	ClearChecker& operator=(const ClearChecker& obj) = delete;

	static ClearChecker* GetInstance();


public:
	void Initialize();
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