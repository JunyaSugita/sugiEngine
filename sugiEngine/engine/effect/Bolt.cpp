#include "Bolt.h"
#include <random>

void Bolt::Initialize(const Vector3& posS, const Vector3& posE, const Vector3& rot, Vector4 color)
{
	for (int i = 0; i < NUM; i++) {
		obj_[i].Initialize("box");
		obj_[i].obj->SetColor(color);
		obj_[i].obj->SetIsSimple();
		obj_[i].obj->SetIsBloom();
		//‘å‚«‚³‚ğ•ÏX
		obj_[i].scale = {0.05f, 0.05f, 0.5f};

		obj_[i].rot = rot;
	}

	Vector3 tempVec = (posE - posS) / NUM;

	//ˆø”‚ÌÀ•W‚ğ“ü‚ê‚é
	for (int i = 0; i < NUM; i++) {
		obj_[i].pos = posS + tempVec * (float)i;
	}
	timer_ = 30;
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
