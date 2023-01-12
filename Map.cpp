#include "Map.h"

Map::~Map()
{
	delete modelBlock;
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			delete objectBlock_[i][j];
		}
	}
	for (int i = 0; i < ITEM_CONST; i++) {
		delete item_[i];
	}
	for (int i = 0; i < TRAP_CONST; i++) {
		delete trap_[i];
	}
	delete goal_;
}

void Map::Initialize(int num)
{
	//オブジェクト
	modelBlock = Model::LoadFromObj("block");
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			objectBlock_[i][j] = Object3d::Create();
			objectBlock_[i][j]->SetModel(modelBlock);
			objectBlock_[i][j]->SetColor(Vector4(1, 1, 1, 1));

			worldTransform_[i][j].scale = { 1,1,1 };
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

	goal_ = new Goal();
	goal_->Initialize();

	SetMap(num);

	//スプライト
	stageNumTexture_[0] = Sprite::LoadTexture("stage0.png");
	stageNumSprite_[0].Initialize(stageNumTexture_[0]);
	stageNumTexture_[1] = Sprite::LoadTexture("stage1.png");
	stageNumSprite_[1].Initialize(stageNumTexture_[1]);
	stageNumTexture_[2] = Sprite::LoadTexture("stage2.png");
	stageNumSprite_[2].Initialize(stageNumTexture_[2]);
	stageNumTexture_[3] = Sprite::LoadTexture("stage3.png");
	stageNumSprite_[3].Initialize(stageNumTexture_[3]);
	stageNumTexture_[4] = Sprite::LoadTexture("stage4.png");
	stageNumSprite_[4].Initialize(stageNumTexture_[4]);
	stageNumTexture_[5] = Sprite::LoadTexture("stage5.png");
	stageNumSprite_[5].Initialize(stageNumTexture_[5]);
	stageNumTexture_[6] = Sprite::LoadTexture("stage6.png");
	stageNumSprite_[6].Initialize(stageNumTexture_[6]);
	stageNumTexture_[7] = Sprite::LoadTexture("stage7.png");
	stageNumSprite_[7].Initialize(stageNumTexture_[7]);
	stageNumTexture_[8] = Sprite::LoadTexture("stage8.png");
	stageNumSprite_[8].Initialize(stageNumTexture_[8]);
	stageNumTexture_[9] = Sprite::LoadTexture("stage9.png");
	stageNumSprite_[9].Initialize(stageNumTexture_[9]);
	stageNumTexture_[10] = Sprite::LoadTexture("stage10.png");
	stageNumSprite_[10].Initialize(stageNumTexture_[10]);
	stageNumTexture_[11] = Sprite::LoadTexture("stage11.png");
	stageNumSprite_[11].Initialize(stageNumTexture_[11]);
	stageNumTexture_[12] = Sprite::LoadTexture("stage12.png");
	stageNumSprite_[12].Initialize(stageNumTexture_[12]);
	stageNumTexture_[13] = Sprite::LoadTexture("stage13.png");
	stageNumSprite_[13].Initialize(stageNumTexture_[13]);
	stageNumTexture_[14] = Sprite::LoadTexture("stage14.png");
	stageNumSprite_[14].Initialize(stageNumTexture_[14]);
	stageNumTexture_[15] = Sprite::LoadTexture("stage15.png");
	stageNumSprite_[15].Initialize(stageNumTexture_[15]);
	stageNumTexture_[16] = Sprite::LoadTexture("stage16.png");
	stageNumSprite_[16].Initialize(stageNumTexture_[16]);
	stageNumTexture_[17] = Sprite::LoadTexture("stage17.png");
	stageNumSprite_[17].Initialize(stageNumTexture_[17]);
	stageNumTexture_[18] = Sprite::LoadTexture("stage18.png");
	stageNumSprite_[18].Initialize(stageNumTexture_[18]);
	stageNumTexture_[19] = Sprite::LoadTexture("stage19.png");
	stageNumSprite_[19].Initialize(stageNumTexture_[19]);
	for (int i = 0; i < 20; i++) {
		stageNumSprite_[i].Size(375,45);
		stageNumSprite_[i].Pos(50,50);
	}

	tutorialTexture_[0] = Sprite::LoadTexture("tutorial1.png");
	tutorialSprite_[0].Initialize(tutorialTexture_[0]);
	tutorialTexture_[1] = Sprite::LoadTexture("tutorial2.png");
	tutorialSprite_[1].Initialize(tutorialTexture_[1]);
	tutorialTexture_[2] = Sprite::LoadTexture("tutorial3.png");
	tutorialSprite_[2].Initialize(tutorialTexture_[2]);
	tutorialTexture_[3] = Sprite::LoadTexture("tutorial4.png");
	tutorialSprite_[3].Initialize(tutorialTexture_[3]);
	tutorialTexture_[4] = Sprite::LoadTexture("tutorial5.png");
	tutorialSprite_[4].Initialize(tutorialTexture_[4]);

	for (int i = 0; i < 5; i++) {
		tutorialSprite_[i].Size(500, 300);
		tutorialSprite_[i].Pos(390,100);
	}
	tutorialSprite_[4].Pos(700, 100);
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
	goal_->Update();
}

void Map::BackSpriteDraw()
{
	if (mapNum_ < 5) {
		tutorialSprite_[mapNum_].Draw();
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
	goal_->Draw();
}

void Map::SpriteDraw()
{
	stageNumSprite_[mapNum_].Draw();
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

Vector3 Map::GetGoalPos()
{
	return goal_->GetPos();
}

void Map::SetMap(int index)
{
	//マップの代入
	mapNum_ = index;

	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			map_[i][j] = map[index][i][j];
		}
	}

	//アイテムの初期化
	for (int i = 0; i < ITEM_CONST; i++) {
		item_[i]->InitializePos();
		item_[i]->SetIsSet(false);
	}
	//トラップの初期化
	for (int i = 0; i < TRAP_CONST; i++) {
		trap_[i]->InitializePos();
		trap_[i]->SetIsSet(false);
	}
	//マップの設置
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			if (map_[i][j] == 1) {
				worldTransform_[i][j].trans.y = (i - 10) * -2;
				worldTransform_[i][j].trans.x = (j - 18) * 2;
				worldTransform_[i][j].trans.z = 0;
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
			else if (map_[i][j] == 5) {
				goal_->SetPos({ float((j - 18) * 2) , float((i - 10) * -2),0 });
				goal_->SetIsSet(true);

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

	//マップの設置
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			objectBlock_[i][j]->SetWorldTransform(worldTransform_[i][j]);
			objectBlock_[i][j]->Update();
		}
	}
}
