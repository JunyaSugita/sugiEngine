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
	LoadSphere("eye");
	Load("slime",true);
	Load("effect");
	LoadSphere("orb");
}

Model* ModelManager::Get(string filename)
{
	map<string, Model*>::iterator it = objects_.find(filename);
	assert(it != objects_.end());

	return it->second;
}

void ModelManager::Load(string filename,bool is)
{
	//�d���`�F�b�N
	if (objects_.find(filename) != objects_.end()) {
		return;
	};

	objects_.insert(make_pair(filename, Model::LoadFromObj(filename,is)));
}

void ModelManager::LoadSphere(string filename)
{
	//�d���`�F�b�N
	if (objects_.find(filename) != objects_.end()) {
		return;
	};

	objects_.insert(make_pair(filename, Model::LoadFromObj("sphere", true)));
}