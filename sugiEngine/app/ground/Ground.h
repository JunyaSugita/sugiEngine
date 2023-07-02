#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Object3d.h"

class Ground {
public:
	static void OneTimeInitialize();
	void Initialize(Vector3 pos, Vector3 scale = Vector3(1,1,1));
	void Update();
	void Draw();

private:
	void WorldTransUpdate();
	void SetWorldTrans();

private:
	static std::unique_ptr<Model> sModel_;

private:
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//–{‘Ì‚Ìƒ‚ƒfƒ‹ŠÖŒW
	std::unique_ptr<Object3d> obj_;
};