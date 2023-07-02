#include "UIManager.h"
#include "Closshair.h"
#include "SpellCharge.h"
#include "SetSpell.h"
#include "Tutorial.h"

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
	SetSpell::GetInstance()->Initialize();
	Tutorial::GetInstance()->Initialize();
}

void UIManager::Update()
{
	Closshair::GetInstance()->Update();
	SpellCharge::GetInstance()->Update();
	SetSpell::GetInstance()->Update();
	Tutorial::GetInstance()->Update();
}

void UIManager::Draw()
{
	Closshair::GetInstance()->Draw();
	SpellCharge::GetInstance()->Draw();
	SetSpell::GetInstance()->Draw();
	Tutorial::GetInstance()->Draw();
}
