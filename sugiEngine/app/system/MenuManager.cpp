#include "MenuManager.h"
#include "Input.h"
#include "Setting.h"

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

	backTex_ = Sprite::LoadTexture("gameMenu_back","png");
	resetTex_ = Sprite::LoadTexture("gameMenu_reset", "png");
	stageSelectTex_ = Sprite::LoadTexture("gameMenu_stageSelect", "png");
	settingTex_ = Sprite::LoadTexture("Setting", "png");

	menuTex_[BACK].Initialize(backTex_);
	menuTex_[RESET].Initialize(resetTex_);
	menuTex_[STAGE_SELECT].Initialize(stageSelectTex_);
	menuTex_[SETTING].Initialize(settingTex_);

	for (int i = 0; i < MAX_MENU; i++) {
		menuTex_[i].SetAnchorPoint(0.5f, 0.5f);
		menuTex_[i].SetSize(SIZE_MENU);
		menuTex_[i].SetPos(500, (float)100 + 150 * i);
	}

	backSp_.Initialize(Sprite::LoadTexture("white1x1", "png"));
	backSp_.SetColor(0, 0, 0, 0.7f);
	backSp_.SetSize(WIN_WIDTH, WIN_HEIGHT);
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

	if (Setting::GetInstance()->GetIsActive()) {
		Setting::GetInstance()->Update();
		return;
	}

	if (GetIsMenu()) {
		timer_--;
		//メニュー共通処理
		if (input->TriggerKey(DIK_W) || input->GetLStickY() > STICK_DELAY || input->PushButton(XINPUT_GAMEPAD_DPAD_UP)) {
			if (selectNum_ > 0 && timer_ <= 0) {
				selectNum_--;
				timer_ = TIME_DELAY;
			}
		}
		if (input->TriggerKey(DIK_S) || input->GetLStickY() < -STICK_DELAY || input->PushButton(XINPUT_GAMEPAD_DPAD_DOWN)) {
			if (selectNum_ < MAX_MENU - 1 && timer_ <= 0) {
				selectNum_++;
				timer_ = TIME_DELAY;
			}
		}
		if (isActive_) {
			if (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) {
				if (selectNum_ == BACK) {
					Back();
				}
				else if (selectNum_ == RESET) {
					isReset_ = true;
				}
				else if (selectNum_ == STAGE_SELECT) {
					isStageSelect_ = true;
				}
				else if (selectNum_ == SETTING) {
					GoToSetting();
				}
			}
		}

		//メニュー共通処理
		for (int i = 0; i < MAX_MENU; i++) {
			if (selectNum_ == i) {
				menuTex_[i].SetSize(SIZE_BIG_MENU);
			}
			else {
				menuTex_[i].SetSize(SIZE_MENU);
			}
		}
	}
}

void MenuManager::Draw()
{
	if (Setting::GetInstance()->GetIsActive()) {
		backSp_.Draw();
		Setting::GetInstance()->Draw();
	}
	else if (isActive_) {
		backSp_.Draw();
		for (int i = 0; i < MAX_MENU; i++) {
			menuTex_[i].Draw();
		}
	}
}

void MenuManager::SetGameMenu()
{
	isActive_ = true;
}

void MenuManager::Back()
{
	isActive_ = false;
}

void MenuManager::GoToSetting()
{
	Setting::GetInstance()->Initialize();
}
