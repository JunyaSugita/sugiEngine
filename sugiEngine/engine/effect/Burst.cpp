#include "Burst.h"
#include <random>

void Burst::Initialize(Vector3 pos, Vector4 color, float range, float pow) {

	sphereModel_ = Model::LoadFromObj("box");
	sphereObj_ = Object3d::Create();
	sphereObj_->SetModel(sphereModel_);

	//�����_��
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> x(-range, range);
	std::uniform_real_distribution<float> y(0.1f, pow);
	std::uniform_real_distribution<float> z(-range, range);

	std::uniform_real_distribution<float> rx(-0.5f, 0.5f);
	std::uniform_real_distribution<float> ry(-0.5f, 0.5f);
	std::uniform_real_distribution<float> rz(-0.5f, 0.0f);
	if (color.x >= 1.1f) {
		std::uniform_real_distribution<float> cx(0.0f, 1.0f);
		color.x = cx(engine);
	}
	if (color.y >= 1.1f) {
		std::uniform_real_distribution<float> cy(0.0f, 1.0f);
		color.y = cy(engine);
	}
	if (color.z >= 1.1f) {
		std::uniform_real_distribution<float> cz(0.0f, 1.0f);
		color.z = cz(engine);
	}
	if (color.w >= 1.1f) {
		std::uniform_real_distribution<float> cw(0.0f, 1.0f);
		color.w = cw(engine);
	}
	color_ = color;

	//�����_���ŏo�����l������
	move_ = { x(engine), y(engine), z(engine) };
	rotate_ = { rx(engine), ry(engine), rz(engine) };

	sphereObj_->SetColor(color_);

	//�����̍��W������
	sphereWorldTransform_.trans_ = pos;
	//�傫����ύX
	sphereWorldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };

	sphereWorldTransform_.SetWorldMat();
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();
}

void Burst::Update() {
	sphereWorldTransform_.trans_ += move_;
	move_.y -= 0.01f;
	if (sphereWorldTransform_.trans_.y < -100) {
		isDead_ = true;
	}
	sphereWorldTransform_.rotation += rotate_;
	sphereWorldTransform_.SetWorldMat();
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();
}

void Burst::Draw() {
	sphereObj_->Draw();
}