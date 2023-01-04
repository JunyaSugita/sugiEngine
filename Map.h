#pragma once
#include "Model.h"
#include "Object3d.h"

class Map
{
public:
	void Initialize();
	void Update();
	void Draw();

	int GetMap(int mapX, int mapY);
private:
	static const int mapX = 37;
	static const int mapY = 21;

	//モデルデータ
	Model* modelBlock;
	Object3d* objectBlock_[mapY][mapX];

	WorldTransform worldTransform_[mapY][mapX];
	int map_[mapY][mapX];
};

