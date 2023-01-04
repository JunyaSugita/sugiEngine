#include "GameScene.h"

void GameScene::Initialize()
{
	catTexture = Sprite::LoadTexture("cat.png");
	sprite.Initialize(catTexture);

	player_ = std::make_unique <Player>();
	player_->Initialize();

	map_ = new Map();
	map_->Initialize();
}

void GameScene::Update(Input* input)
{
	map_->Update();
	player_->Update(input,map_);

	if (input->TriggerKey(DIK_1)) {
		GameManager::SetTitleScene();
	}
}

void GameScene::Draw()
{
	map_->Draw();
	player_->Draw();
}

void GameScene::SpriteDraw()
{
	sprite.Draw();
}

void GameScene::Delete()
{
	delete map_;
}
