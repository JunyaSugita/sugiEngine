#include "Burst.h"
#include <random>

void Burst::Initialize(Vector3 pos, Vector4 color, float range, float pow) {

	model_ = Model::LoadFromObj("box");
	obj_ = Object3d::Create();
	obj_->SetModel(model_);

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

	obj_->SetColor(color_);

	//引数の座標を入れる
	worldTransform_.trans = pos;
	//大きさを変更
	worldTransform_.scale = { 1.0f, 1.0f, 1.0f };

	worldTransform_.SetWorldMat();
	obj_->SetWorldTransform(worldTransform_);
	obj_->Update();
}

void Burst::Update() {
	worldTransform_.trans += move_;
	move_.y -= 0.01f;
	if (worldTransform_.trans.y < -100) {
		isDead_ = true;
	}
	worldTransform_.rotation += rotate_;
	worldTransform_.SetWorldMat();
	obj_->SetWorldTransform(worldTransform_);
	obj_->Update();
}

void Burst::Draw() {
	obj_->Draw();
}