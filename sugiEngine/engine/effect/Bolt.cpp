#include "Bolt.h"
#include <random>

void Bolt::Initialize(const Vector3& posS, const Vector3& posE, const Vector3& rot, Vector4 color)
{
	for (int i = 0; i < NUM; i++) {
		obj_[i].Initialize("box");
		obj_[i].obj->SetColor(color);
		obj_[i].obj->SetIsSimple();
		obj_[i].obj->SetIsBloom();
		//大きさを変更
		obj_[i].scale = { SIZE, SIZE, SIZE };

		obj_[i].rot = rot;
	}

	Vector3 tempVec = (posE - posS) / NUM;

	//引数の座標を入れる
	for (int i = 0; i < NUM; i++) {
		obj_[i].pos = posS + tempVec * (float)i;
	}
	timer_ = TIME;
}

void Bolt::Update()
{
	if (--timer_ < 0) {
		isDead_ = true;
	}
	
	for (int i = 0; i < NUM; i++) {
		obj_[i].Update();
	}
}

void Bolt::Draw()
{
	for (int i = 0; i < NUM; i++) {
		obj_[i].Draw();
	}
}
