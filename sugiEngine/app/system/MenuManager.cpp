#include "MenuManager.h"
#include "Input.h"

MenuManager* MenuManager::GetInstance()
{
	static MenuManager instance;

	return &instance;
}

void MenuManager::Initialize()
{
	isActive_ = false;
	for (int i = 0; i < TEX_NUM; i++) {
		menuTex_[i].Initialize(Sprite::LoadTexture("white1x1.png"));
		menuTex_[i].SetSize(300,100);
		menuTex_[i].SetPos(100,(float)200 + 300 * i);
	}
}

void MenuManager::Update()
{
	Input* input = Input::GetInstance();

	if (isActive_) {
		if(input->TriggerKey(DIK_S)) {
			if (selectNum_ > 0) {
				selectNum_--;
			}
		}
		if (input->TriggerKey(DIK_W)) {
			if (selectNum_ < menuNum_ - 1) {
				selectNum_++;
			}
		}

		if (input->TriggerKey(DIK_A)) {
			Back();
		}
	}
}

void MenuManager::Draw()
{
	if (isActive_) {
		for (int i = 0; i < TEX_NUM; i++) {
			menuTex_[i].Draw();
		}
	}
}

void MenuManager::SetGameManu()
{
	isActive_ = true;
	menuNum_ = 3;
}

void MenuManager::Back()
{
	SetIsActive(false);
}

void MenuManager::Reset()
{
}

void MenuManager::GoStageSelect()
{
}

void MenuManager::GoSpellSetting()
{
}

void MenuManager::BackStage()
{
}

void MenuManager::Enter()
{
}

void MenuManager::Cancel()
{
}
