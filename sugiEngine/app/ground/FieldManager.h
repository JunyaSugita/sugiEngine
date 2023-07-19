#pragma once
#include "Model.h"
#include "Object3d.h"
#include "JsonLoader.h"
#include "ColliderManager.h"

class FieldManager {
private:
	FieldManager();
	~FieldManager();

public:
	//�R�s�[�R���X�g���N�^����
	FieldManager(const FieldManager& obj) = delete;
	//������Z�q�𖳌�
	FieldManager& operator=(const FieldManager& obj) = delete;

	static FieldManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	vector<BoxCol> GetCol() {
		return col_;
	}

private:
	LevelData* levelData_;

	std::unique_ptr<Model> boxModel_;
	std::unique_ptr<Object3d> object_[100];

	std::vector<BoxCol> col_;

	int32_t objNum_ = 0;
};