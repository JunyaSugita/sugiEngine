#include "GameManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"

GameManager::GameManager()
{
	scene_ = std::make_unique<GameScene>();
	scene_->Initialize();

	Camera::GetInstance()->Initialize();
}

GameManager::~GameManager()
{
}

void GameManager::Update()
{
	//各シーンの処理
	scene_->Update();
	//カメラ処理
	Camera::GetInstance()->Update();
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

GameManager* GameManager::GetInstance()
{
	static GameManager instance;

	return &instance;
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

void GameManager::SetClearScene()
{
	scene_->Delete();
	scene_ = std::make_unique<ClearScene>();
	scene_->Initialize();
}
