#include "UIManager.h"
#include "Closshair.h"
#include "SpellCharge.h"
#include "SetSpell.h"
#include "Tutorial.h"
#include "sceneChange.h"

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
	SceneChange::GetInstance()->Initialize();

	clearTex_ = Sprite::LoadTexture("gameClear.png");
	gameOverTex_ = Sprite::LoadTexture("gameOver.png");

	stateSp_.Initialize(clearTex_);
	stateSp_.SetAnchorPoint(0.5f,0.5f);
	stateSp_.SetPos(WIN_WIDTH / 2,WIN_HEIGHT / 2);
	stateAlpha_ = 0;
	stateSp_.SetColor(1,1,1, stateAlpha_);
}

void UIManager::Update()
{
	Closshair::GetInstance()->Update();
	SpellCharge::GetInstance()->Update();
	SetSpell::GetInstance()->Update();
	Tutorial::GetInstance()->Update();
	SceneChange::GetInstance()->Update();

	stateSp_.SetColor(1, 1, 1, stateAlpha_);
}

void UIManager::Draw()
{
	Closshair::GetInstance()->Draw();
	SpellCharge::GetInstance()->Draw();
	SetSpell::GetInstance()->Draw();
	//Tutorial::GetInstance()->Draw();
	stateSp_.Draw();


	SceneChange::GetInstance()->Draw();
}

void UIManager::SetClear()
{
	stateSp_.SetTexture(clearTex_);
	stateAlpha_ += 0.025f;
}

void UIManager::SetGameOver()
{
	stateSp_.SetTexture(gameOverTex_);
	stateAlpha_ += 0.025f;
}
