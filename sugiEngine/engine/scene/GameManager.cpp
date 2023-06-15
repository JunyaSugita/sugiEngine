#include "GameManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"

GameManager::GameManager()
{
	scene_ = std::make_unique<GameScene>();
	scene_->Initialize();
}

GameManager::~GameManager()
{
}

void GameManager::Update()
{
	//ŠeƒV[ƒ“‚Ìˆ—
	scene_->Update();
}

void GameManager::BackSpriteDraw()
{
	scene_->BackSpriteDraw();
}

void GameManager::Draw()
{
	scene_->Draw();
}

void GameManager::ObjDraw()
{
	scene_->ObjDraw();
}

void GameManager::SpriteDraw()
{
	scene_->SpriteDraw();
}

void GameManager::Finalize()
{
	scene_->Finalize();
}

GameManager* GameManager::GetInstance()
{
	static GameManager instance;

	return &instance;
}

void GameManager::SetTitleScene()
{
	scene_->Finalize();
	scene_ = std::make_unique<TitleScene>();
	scene_->Initialize();
}

void GameManager::SetGameScene()
{
	scene_->Finalize();
	scene_ = std::make_unique<GameScene>();
	scene_->Initialize();
}

void GameManager::SetClearScene()
{
	scene_->Finalize();
	scene_ = std::make_unique<ClearScene>();
	scene_->Initialize();
}
