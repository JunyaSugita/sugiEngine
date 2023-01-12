#include "GameScene.h"

void GameScene::Initialize(int num)
{
	player_ = std::make_unique <Player>();
	player_->Initialize(num);

	map_ = new Map();
	map_->Initialize(num);
}

void GameScene::Update(Input* input)
{
	map_->Update();

	int num = player_->Update(input, map_);

	if (num != 0) {
		GameManager::SetStage(num);
		GameManager::SetClearScene();
	}
}

void GameScene::BackSpriteDraw()
{
	map_->BackSpriteDraw();
}

void GameScene::Draw()
{
	map_->Draw();
	player_->Draw();
}

void GameScene::SpriteDraw()
{
	map_->SpriteDraw();
}

void GameScene::Delete()
{
	player_->Delete();

	delete map_;
}
