#include "GameManager.h"
#include "TitleScene.h"
#include "GameScene.h"

std::unique_ptr<ISceneManager> GameManager::scene_;

GameManager::GameManager()
{
	scene_ = std::make_unique<TitleScene>();
	scene_->Initialize();
}

void GameManager::Update(Input* input)
{
	scene_->Update(input);
}

void GameManager::Draw()
{
	scene_->Draw();
}

void GameManager::SpriteDraw()
{
	scene_->SpriteDraw();
}

void GameManager::Delete()
{
	scene_->Delete();
}

void GameManager::SetTitleScene()
{
	scene_->Delete();
	scene_ = std::make_unique<TitleScene>();
	scene_->Initialize();
}

void GameManager::SetGameScene()
{
	scene_->Delete();
	scene_ = std::make_unique<GameScene>();
	scene_->Initialize();
}
