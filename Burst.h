#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "Object3d.h"

class Burst {
public:
	void Initialize(Vector3 pos,Vector4 color, float range, float pow);
	void Update();
	void Draw();

	bool IsDead() const {
		return isDead_;
	}
private:
	//座標
	WorldTransform worldTransform_;
	//移動
	Vector3 move_;
	//色
	Vector4 color_;
	//回転
	Vector3 rotate_;
	//モデル
	Model* model_;
	//テクスチャ
	Object3d* obj_;
	//消滅フラグ
	bool isDead_;
};
