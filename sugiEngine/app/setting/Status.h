#pragma once
#include <vector>
#include "BaseSpell.h"

class Status {
private:
	Status() = default;
	~Status() = default;

public:
	//コピーコンストラクタ無効
	Status(const Status& obj) = delete;
	//代入演算子を無効
	Status& operator=(const Status& obj) = delete;

	static Status* GetInstance();

public:
	void LoadJson();

	SpellData GetSpellData(int32_t spellNum) {
		return spellData_[spellNum];
	}

private:
	//呪文ステータス群
	std::vector<SpellData> spellData_;
};