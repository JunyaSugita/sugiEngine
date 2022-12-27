#pragma once
#include "Model.h"
#include "Object3d.h"

class Map
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	//モデルデータ
	Model* modelBlock;
	Object3d* objectBlock_[20][30];

	WorldTransform worldTransform_[20][30];
	int map_[20][30];
};

