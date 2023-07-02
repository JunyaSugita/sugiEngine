#include "Burst.h"
#include <random>

std::unique_ptr <Model> Burst::sSphereModel_;

void Burst::OneTimeInitialize()
{
	sSphereModel_ = move(Model::LoadFromObj("box"));
}

void Burst::Initialize(const Vector3& pos,Vector4 color,float range,float pow) {

	sphereObj_ = move(Object3d::Create());
	sphereObj_->SetModel(sSphereModel_.get());

	//ランダム
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

	//ランダムで出した値を入れる
	move_ = { x(engine), y(engine), z(engine) };
	rotate_ = { rx(engine), ry(engine), rz(engine) };

	sphereObj_->SetColor(color_);

	//引数の座標を入れる
	sphereWorldTransform_.SetPos(pos);
	//大きさを変更
	sphereWorldTransform_.SetScale(Vector3(0.1f, 0.1f, 0.1f));

	sphereWorldTransform_.SetWorldMat();
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();
}

void Burst::Update() {
	sphereWorldTransform_.AddPos(move_);
	move_.y -= 0.01f;
	if (sphereWorldTransform_.GetPos().y < -1) {
		isDead_ = true;
	}
	sphereWorldTransform_.AddRot(rotate_);
	sphereWorldTransform_.SetWorldMat();
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();
}

void Burst::Draw() {
	sphereObj_->Draw();
}