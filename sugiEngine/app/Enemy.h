#pragma once
#include "Sugimath.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Object3d.h"

class Enemy{
public:
	void Initialize();
	void Update();
	void Draw();

	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	}

private:
	void WorldTransUpdate();
	void SetWorldTrans();

private:
	const float SPEED_MOVE = 0.05f;
	const float SPEED_ANGLE = 1;
	const float RADIAN = 180;

private:
	//–{‘Ì
	WorldTransform worldTransform_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	bool isDead_;

	std::unique_ptr<Model> model_;
	std::unique_ptr<Object3d> obj_;

	//–Ú
	WorldTransform eyeWorldTransform_;
	Vector3 eyePos_;
	Vector3 eyeRot_;
	Vector3 eyeScale_;
	std::unique_ptr<Model> eyeModel_;
	std::unique_ptr<Object3d> eyeObj_;
};