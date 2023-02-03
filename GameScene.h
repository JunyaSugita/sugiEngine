#pragma once
#include "ISceneManager.h"
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"
#include "CollisionPrimitive.h"

class GameScene : public ISceneManager
{
public:
	void Initialize(int num) override;
	void Update(Input* input) override;
	void BackSpriteDraw() override;
	void Draw() override;
	void SpriteDraw() override;
	void Delete() override;

public:

private:
	//sphere
	Model* sphereModel_;
	Object3d* sphereObj_;
	WorldTransform sphereWorldTransform_;
	Sphere sphere_;
	//plane
	Model* planeModel_;
	Object3d* planeObj_;
	WorldTransform planeWorldTransform_;
	Plane plane_;
	//triangle
	Model* triangleModel_;
	Object3d* triangleObj_;
	WorldTransform triangleWorldTransform_;
	Triangle triangle_;
	//ray
	Model* rayModel_;
	Object3d* rayObj_;
	WorldTransform rayWorldTransform_;
	Ray ray_;
	//sphere2
	Model* sphere2Model_;
	Object3d* sphere2Obj_;
	WorldTransform sphere2WorldTransform_;
	Sphere sphere2_;

	int scene_ = 0;
	bool isMove_ = 0;
	static const int TIME_MAX = 50;
	int timer_ = TIME_MAX;
};