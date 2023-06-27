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

private:
	void WorldTransUpdate();
	void SetWorldTrans();

private:
	WorldTransform worldTransform_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	std::unique_ptr<Model> model_;
	std::unique_ptr<Object3d> obj_;
};