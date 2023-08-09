#include "BaseObj.h"
#include "ModelManager.h"

using namespace std;

void BaseObj::Initialize(string str)
{
	obj = move(Object3d::Create());
	obj->SetModel(ModelManager::GetInstance()->Get(str));
	
	pos = { 0,0,0 };
	rot = { 0,0,0 };
	scale = { 1,1,1 };

	worldTrans.SetPos(pos);
	worldTrans.SetRot(rot);
	worldTrans.SetScale(scale);
	obj->SetWorldTransform(worldTrans);
}

void BaseObj::Update()
{
	worldTrans.SetPos(pos);
	worldTrans.SetRot(rot);
	worldTrans.SetScale(scale);
	obj->SetWorldTransform(worldTrans);
	obj->Update();
}

void BaseObj::Draw()
{
	obj->Draw();
}
