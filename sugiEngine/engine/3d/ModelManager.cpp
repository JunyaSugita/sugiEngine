#include "ModelManager.h"

using namespace std;

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;

	return &instance;
}

unique_ptr<Model> ModelManager::Get(std::string filename)
{
	map<string, unique_ptr<Model>>::iterator it = objects_.find(filename);
	assert(it != objects_.end());

	return move(it->second);
}

void ModelManager::Load(std::string filename,bool is)
{
	//重複チェック
	if (objects_.find(filename) != objects_.end()) {
		return;
	};

	objects_.insert(std::make_pair(filename, Model::LoadFromObj(filename,is)));
}
