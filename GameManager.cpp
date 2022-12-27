#include "GameManager.h"
#include "TitleScene.h"
#include "GameScene.h"

std::unique_ptr<ISceneManager> GameManager::scene_;

GameManager::GameManager()
{
	SetTitleScene();
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

void GameManager::SetTitleScene()
{
	scene_ = std::make_unique<TitleScene>();
	scene_->Initialize();
}

void GameManager::SetGameScene()
{
	scene_ = std::make_unique<GameScene>();
	scene_->Initialize();
}
