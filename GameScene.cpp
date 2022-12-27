#include "GameScene.h"

void GameScene::Initialize()
{
	catTexture = Sprite::LoadTexture("cat.png");
	sprite.Initialize(catTexture);

	player_ = std::make_unique <Player>();
	player_->Initialize();

	map_ = std::make_unique <Map>();
	map_->Initialize();
}

void GameScene::Update(Input* input)
{
	player_->Update(input);
	map_->Update();

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
