#include "Map.h"

Map::~Map()
{
	for (int i = 0; i < ITEM_CONST; i++) {
		delete item_[i];
	}
	for (int i = 0; i < TRAP_CONST; i++) {
		delete trap_[i];
	}
}

void Map::Initialize()
{
	//オブジェクト
	modelBlock = Model::LoadFromObj("box");
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			objectBlock_[i][j] = Object3d::Create();
			objectBlock_[i][j]->SetModel(modelBlock);
			objectBlock_[i][j]->SetColor(Vector4(0, 0, 0, 1));

			worldTransform_[i][j].scale = { 10,10,10 };
		}
	}

	//アイテムの初期化
	for (int i = 0; i < ITEM_CONST; i++) {
		item_[i] = new Item();
		item_[i]->Initialize();
	}
	//トラップの初期化
	for (int i = 0; i < TRAP_CONST; i++) {
		trap_[i] = new Trap();
		trap_[i]->Initialize();
	}
	
	SetMap();
}

void Map::Update()
{
	for (int i = 0; i < ITEM_CONST; i++) {
		if (item_[i]->GetIsSet() == true) {
			item_[i]->Update();
		}
	}
	for (int i = 0; i < TRAP_CONST; i++) {
		if (trap_[i]->GetIsSet() == true) {
			trap_[i]->Update();
		}
	}
}

void Map::Draw()
{
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			objectBlock_[i][j]->Draw();
		}
	}
	for (int i = 0; i < ITEM_CONST; i++) {
		if (item_[i]->GetIsSet() == true) {
			item_[i]->Draw();
		}
	}
	for (int i = 0; i < TRAP_CONST; i++) {
		if (trap_[i]->GetIsSet() == true) {
			trap_[i]->Draw();
		}
	}
}

int Map::GetMap(int mapX, int mapY)
{
	return map_[mapY][mapX];
}

Vector3 Map::GetItemPos(int index)
{
	return item_[index]->GetPos();
}

Vector3 Map::GetTrapPos(int index)
{
	return trap_[index]->GetPos();
}

void Map::SetMap()
{
	//マップの代入
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			map_[i][j] = map1[i][j];
		}
	}

	//マップの設置
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			if (map_[i][j] == 1) {
				worldTransform_[i][j].trans.y = (i - 10) * -2;
				worldTransform_[i][j].trans.x = (j - 18) * 2;
			}
			else if (map_[i][j] == 2) {
				for (int k = 0; k < ITEM_CONST; k++) {
					if (item_[k]->GetIsSet() == false) {
						item_[k]->SetPos({ float((j - 18) * 2) , float((i - 10) * -2),0 });
						item_[k]->SetIsSet(true);
						break;
					}
				}

				worldTransform_[i][j].trans.y = (i - 10) * -2;
				worldTransform_[i][j].trans.x = (j - 18) * 2;
				worldTransform_[i][j].trans.z = -1000;
			}
			else if (map_[i][j] == 3) {
				for (int k = 0; k < TRAP_CONST; k++) {
					if (trap_[k]->GetIsSet() == false) {
						trap_[k]->SetPos({ float((j - 18) * 2) , float((i - 10) * -2),0 });
						trap_[k]->SetIsSet(true);
						break;
					}
				}

				worldTransform_[i][j].trans.y = (i - 10) * -2;
				worldTransform_[i][j].trans.x = (j - 18) * 2;
				worldTransform_[i][j].trans.z = -1000;
			}
			else {
				worldTransform_[i][j].trans.y = (i - 10) * -2;
				worldTransform_[i][j].trans.x = (j - 18) * 2;
				worldTransform_[i][j].trans.z = -1000;
			}

			if (map_[i][j] == 4) {
				respornPoint_ = { float((j - 18) * 2) , float((i - 10) * -2),0 };
			}
		}
	}

	//マップ0の設置
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			objectBlock_[i][j]->SetWorldTransform(worldTransform_[i][j]);
			objectBlock_[i][j]->Update();
		}
	}
}
