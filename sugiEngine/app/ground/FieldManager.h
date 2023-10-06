#pragma once
#include "Model.h"
#include "BaseObj.h"
#include "JsonLoader.h"
#include "ColliderManager.h"

class FieldManager final{
private:
	FieldManager() = default;
	~FieldManager() = default;

public:
	//コピーコンストラクタ無効
	FieldManager(const FieldManager& obj) = delete;
	//代入演算子を無効
	FieldManager& operator=(const FieldManager& obj) = delete;

	static FieldManager* GetInstance();

public:
	void Initialize(int num);
	void Update();
	void Draw();

	BoxCol GetCol(int num) {
		return col_[num];
	}

	size_t GetColSize() {
		return col_.size();
	}

	int32_t GetNavePointNum() {
		return navePointNum_;
	}

	void SelectStage(int num);

private:
	LevelData* levelData_;

	std::unique_ptr<Model> boxModel_;
	BaseObj obj_[100];

	std::vector<BoxCol> col_;

	int32_t objNum_;
	int32_t navePointNum_;
};
