#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "Object3d.h"

class Burst {
public:
	static void OneTimeInitialize();
	void Initialize(const Vector3& pos,Vector4 color, float range,float pow);
	void Update();
	void Draw();

	bool IsDead() const {
		return isDead_;
	}
private:
	//モデル
	static std::unique_ptr <Model> sSphereModel_;

private:
	//座標
	WorldTransform sphereWorldTransform_;
	//移動
	Vector3 move_;
	//色
	Vector4 color_;
	//回転
	Vector3 rotate_;
	//テクスチャ
	std::unique_ptr<Object3d> sphereObj_;
	//消滅フラグ
	bool isDead_;
};
