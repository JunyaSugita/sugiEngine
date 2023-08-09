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
	//�R�s�[�R���X�g���N�^����
	FieldManager(const FieldManager& obj) = delete;
	//������Z�q�𖳌�
	FieldManager& operator=(const FieldManager& obj) = delete;

	static FieldManager* GetInstance();

public:
	void Initialize();
	void GameInitialize();
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

private:
	LevelData* levelData_;

	std::unique_ptr<Model> boxModel_;
	//std::unique_ptr<Model> groundModel_;
	//std::unique_ptr<Object3d> object_[100];
	BaseObj obj_[100];

	std::vector<BoxCol> col_;

	int32_t objNum_;
	int32_t navePointNum_;
};