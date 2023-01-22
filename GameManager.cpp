#include "GameManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"

std::unique_ptr<ISceneManager> GameManager::scene_;
int GameManager::stage_;

GameManager::GameManager()
{
	scene_ = std::make_unique<GameScene>();
	scene_->Initialize(0);
}

void GameManager::Update(Input* input)
{
	scene_->Update(input);
}

void GameManager::BackSpriteDraw()
{
	scene_->BackSpriteDraw();
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
	scene_->Initialize(0);
}

void GameManager::SetGameScene()
{
	scene_->Delete();
	scene_ = std::make_unique<GameScene>();
	scene_->Initialize(stage_);
}

void GameManager::SetClearScene()
{
	scene_->Delete();
	scene_ = std::make_unique<ClearScene>();
	scene_->Initialize(stage_);
}
