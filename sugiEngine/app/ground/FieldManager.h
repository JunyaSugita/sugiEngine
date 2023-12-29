/**
 * @file FieldManager.h
 * @brief フィールドを生成する
 */
#pragma once
#include "Model.h"
#include "BaseObj.h"
#include "JsonLoader.h"
#include "ColliderManager.h"
#include "Torch.h"

class FieldManager final {
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

	int32_t GetNaviPointNum() {
		return navePointNum_;
	}

	void SelectStage(int num);

	static void SetLight(LightGroup* lightGroup);

private:
	//ライトの明るさ
	const float ATTEN_LOW = 0.01f;
	const float ATTEN_HIGT = 0.001f;
	//ゴール関係
	const Vector3 COLOR_GOAL = {0,0,1};
	const float ATTEN_GOAL = 0.002f;

	LevelData* levelData_;

	std::unique_ptr<Model> boxModel_;
	std::vector <std::unique_ptr<BaseObj>> objs_;
	std::vector<BoxCol> col_;

	std::vector<std::unique_ptr<Torch>> torchs_;

	int32_t navePointNum_;

	//ライト関係
	static LightGroup* lightGroup_;
	int32_t useLightNum_;
	float stageAtten_;
};
