#pragma once
#include "GrovalSetting.h"
#include "Model.h"
#include <map>

class ModelManager final {
private:
	ModelManager() = default;
	~ModelManager() = default;
public:
	//�R�s�[�R���X�g���N�^����
	ModelManager(const ModelManager& obj) = delete;
	//������Z�q�𖳌�
	ModelManager& operator=(const ModelManager& obj) = delete;

	static ModelManager* GetInstance();

public:
	std::unique_ptr<Model> Get(std::string filename);
	void Load(std::string filename,bool is = false);

private:
	std::map<std::string,std::unique_ptr<Model>> objects_;
};