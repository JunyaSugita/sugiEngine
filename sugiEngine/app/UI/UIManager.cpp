#include "UIManager.h"
#include "Closshair.h"
#include "SpellCharge.h"

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
	SpellCharge::GetInstance()->Initialize();
}

void UIManager::Update()
{
	Closshair::GetInstance()->Update();
	SpellCharge::GetInstance()->Update();
}

void UIManager::Draw()
{
	Closshair::GetInstance()->Draw();
	SpellCharge::GetInstance()->Draw();
}
