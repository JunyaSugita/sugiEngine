#include "Burst.h"
#include <random>
#include "ModelManager.h"

void Burst::Initialize(const Vector3& pos,Vector4 color,float range,float pow) {

	obj_.Initialize("effect");
	obj_.obj->SetColor(color);

	//�����_��
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> x(-range, range);
	std::uniform_real_distribution<float> y(0.1f, pow);
	std::uniform_real_distribution<float> z(-range, range);

	std::uniform_real_distribution<float> rx(-0.5f, 0.5f);
	std::uniform_real_distribution<float> ry(-0.5f, 0.5f);
	std::uniform_real_distribution<float> rz(-0.5f, 0.0f);

	//�����_���ŏo�����l������
	move_ = { x(engine), y(engine), z(engine) };
	rotate_ = { rx(engine), ry(engine), rz(engine) };

	//�����̍��W������
	obj_.pos = pos;
	//�傫����ύX
	obj_.scale = { 0.1f, 0.1f, 0.1f };
}

void Burst::Update() {
	obj_.pos += move_;
	move_.y -= 0.01f;
	if (obj_.pos.y < -1) {
		isDead_ = true;
	}
	obj_.rot += rotate_;
	obj_.Update();
}

void Burst::Draw() {
	obj_.Draw();
}