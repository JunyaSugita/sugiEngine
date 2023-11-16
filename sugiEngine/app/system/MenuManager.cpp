#include "MenuManager.h"
#include "Input.h"

MenuManager* MenuManager::GetInstance()
{
	static MenuManager instance;

	return &instance;
}

bool MenuManager::GetIsMenu()
{
	if (isActive_ || isStageSelectBackCheck_ || isResetCheck_) {
		return true;
	}
	return false;
}

void MenuManager::Initialize()
{
	GameInitialize();

	backTex_ = Sprite::LoadTexture("gameMenu_back.png");
	resetTex_ = Sprite::LoadTexture("gameMenu_reset.png");
	stageSelectTex_ = Sprite::LoadTexture("gameMenu_stageSelect.png");

	menuTex_[0].Initialize(backTex_);
	menuTex_[1].Initialize(resetTex_);
	menuTex_[2].Initialize(stageSelectTex_);


	for (int i = 0; i < MAX_MENU; i++) {
		menuTex_[i].SetAnchorPoint(0.5f, 0.5f);
		menuTex_[i].SetSize(500, 150);
		menuTex_[i].SetPos(500, (float)100 + 250 * i);
	}

	backSp_.Initialize(Sprite::LoadTexture("white1x1.png"));
	backSp_.SetColor(0,0,0,0.7f);
	backSp_.SetSize(1280,720);
}

void MenuManager::GameInitialize()
{
	isActive_ = false;
	isReset_ = false;
	isStageSelect_ = false;
	isResetCheck_ = false;
	isStageSelectBackCheck_ = false;
	selectNum_ = 0;
	checkNum_ = 0;
	timer_ = 0;
}

void MenuManager::Update()
{
	Input* input = Input::GetInstance();

	if (GetIsMenu()) {
		timer_--;
		//メニュー共通処理
		if (input->TriggerKey(DIK_W) || input->GetLStickY() > 10000 || input->PushButton(XINPUT_GAMEPAD_DPAD_UP)) {
			if (selectNum_ > 0 && timer_ <= 0) {
				selectNum_--;
				timer_ = 10;
			}
		}
		if (input->TriggerKey(DIK_S) || input->GetLStickY() < -10000 || input->PushButton(XINPUT_GAMEPAD_DPAD_DOWN)) {
			if (selectNum_ < menuNum_ - 1 && timer_ <= 0) {
				selectNum_++;
				timer_ = 10;
			}
		}

		/*if (isResetCheck_) {
			if (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) {
				if (selectNum_ == 0) {
					isStageSelectBackCheck_ = false;
					selectNum_ = checkNum_;
					menuNum_ = 3;
				}
				else if (selectNum_ == 1) {
					isReset_ = true;
				}
			}
		}
		else if (isStageSelectBackCheck_) {
			if (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) {
				if (selectNum_ == 0) {
					isStageSelectBackCheck_ = false;
					selectNum_ = checkNum_;
					menuNum_ = 3;
				}
				else if (selectNum_ == 1) {
					isStageSelect_ = true;
				}
			}
		}
		else */if (isActive_) {
			if (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) {
				if (selectNum_ == 0) {
					Back();
				}
				else if (selectNum_ == 1) {
					//SetResetCheck();
					isReset_ = true;
				}
				else if (selectNum_ == 2) {
					//SetStageSelectBackCheck();
					isStageSelect_ = true;
				}
			}
		}

		//メニュー共通処理
		for (int i = 0; i < menuNum_; i++) {
			if (selectNum_ == i) {
				menuTex_[i].SetSize(550, 165);
			}
			else {
				menuTex_[i].SetSize(500, 150);
			}
		}
	}
}

void MenuManager::Draw()
{
	if (isActive_) {
		backSp_.Draw();
		for (int i = 0; i < menuNum_; i++) {
			menuTex_[i].Draw();
		}
	}
}

void MenuManager::SetGameMenu()
{
	isActive_ = true;
	menuNum_ = 3;
}

void MenuManager::SetResetCheck()
{
	isResetCheck_ = true;
	menuNum_ = 2;
	checkNum_ = selectNum_;
	selectNum_ = 0;
}

void MenuManager::SetStageSelectBackCheck()
{
	isStageSelectBackCheck_ = true;
	menuNum_ = 2;
	checkNum_ = selectNum_;
	selectNum_ = 0;
}

void MenuManager::Back()
{
	isActive_ = false;
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
