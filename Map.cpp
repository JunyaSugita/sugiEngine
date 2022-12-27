#include "Map.h"

void Map::Initialize()
{
	//オブジェクト
	modelBlock = Model::LoadFromObj("box");
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++) {
			objectBlock_[i][j] = Object3d::Create();
			objectBlock_[i][j]->SetModel(modelBlock);

			worldTransform_[i][j].scale = { 10,10,10 };
		}
	}

	//マップ
	int map1[20][30] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++) {
			map_[i][j] = map1[i][j];
		}
	}
}

void Map::Update()
{

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++) {
			if (map_[i][j] == 1) {
				worldTransform_[i][j].trans.y = (i - 10) * 2;
				worldTransform_[i][j].trans.x = (j - 15) * 2;
			}
			else {
				worldTransform_[i][j].trans.y = (i - 10) * 3.2f;
				worldTransform_[i][j].trans.x = (j - 15) * 3.2f;
				worldTransform_[i][j].trans.z = -1000;
			}
		}
	}

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++) {
			objectBlock_[i][j]->SetWorldTransform(worldTransform_[i][j]);
			objectBlock_[i][j]->Update();
		}
	}
}

void Map::Draw()
{
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 30; j++) {
			objectBlock_[i][j]->Draw();
		}
	}
}
