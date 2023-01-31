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
	Model* sphereModel_;
	Object3d* sphereObj_;
	WorldTransform sphereWorldTransform_;
	Sphere sphere_;

	Model* planeModel_;
	Object3d* planeObj_;
	WorldTransform planeWorldTransform_;
	Plane plane_;
};

