/**
 * @file ModelManager.h
 * @brief モデルを管理するマネージャー
 */

#pragma once
#include "GlobalSetting.h"
#include "Model.h"
#include <map>

class ModelManager final {
private:
	ModelManager() = default;
	~ModelManager() = default;
public:
	//コピーコンストラクタ無効
	ModelManager(const ModelManager& obj) = delete;
	//代入演算子を無効
	ModelManager& operator=(const ModelManager& obj) = delete;

	static ModelManager* GetInstance();

public:
	void Initialize();
	Model* Get(std::string filename);
	void Load(std::string filename,bool is = false);
	void LoadSphere(std::string filename);

private:
	std::map<std::string,Model*> objects_;
};
