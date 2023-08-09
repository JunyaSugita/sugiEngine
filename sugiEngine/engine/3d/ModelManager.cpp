#include "ModelManager.h"

using namespace std;

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;

	return &instance;
}

void ModelManager::Initialize()
{
	Load("player");
	Load("box");
	Load("sphere", true);
	Load("weapon");
	Load("ground");
	Load("eye",true);
}

Model* ModelManager::Get(std::string filename)
{
	map<string, Model*>::iterator it = objects_.find(filename);
	assert(it != objects_.end());

	return it->second;
}

void ModelManager::Load(std::string filename,bool is)
{
	//重複チェック
	if (objects_.find(filename) != objects_.end()) {
		return;
	};

	objects_.insert(std::make_pair(filename, Model::LoadFromObj(filename,is)));
}
