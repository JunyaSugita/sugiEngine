#include "UIManager.h"
#include "Closshair.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

UIManager* UIManager::GetInstance()
{
	static UIManager instance;

	return &instance;
}

void UIManager::Initialize()
{
	Closshair::GetInstance()->Initialize();
}

void UIManager::Update()
{
	Closshair::GetInstance()->Update();
}

void UIManager::Draw()
{
	Closshair::GetInstance()->Draw();
}
