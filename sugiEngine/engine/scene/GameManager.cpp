#include "GameManager.h"

GameManager::GameManager()
{
	scene_ = std::make_unique<TitleScene>();
	scene_->Initialize();
}
void GameManager::Update()
{
	//各シーンの処理
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

void GameManager::DrawTransparent()
{
	scene_->DrawTransparent();
}

void GameManager::ParticleDraw()
{
	scene_->ParticleDraw();
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

void GameManager::ChangeScene(std::unique_ptr<IScene> scene)
{
	scene_->Finalize();
	scene_ = move(scene);
	scene_->Initialize();
}