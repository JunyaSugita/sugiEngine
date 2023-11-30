#include "Setting.h"
#include "Input.h"

void Setting::Initialize()
{
	isSetting_ = true;
	menuNum_ = 0;
	timer_ = 0;
}

void Setting::Update()
{
	//設定画面中のみ操作可能
	if (isSetting_) {

		//インスタンス呼び出し
		Input* input = Input::GetInstance();

		//タイマーを減らす
		if (timer_ > 0) {
			timer_--;
		}

		if ((input->GetLStickY() < -10000 || input->PushButton(XINPUT_GAMEPAD_DPAD_DOWN)) && timer_ <= 0) {
			if (menuNum_ < MAX_MENU) {
				menuNum_++;
				timer_ = TIME_MOVE;
			}
		}
		if ((input->GetLStickY() > 10000 || input->PushButton(XINPUT_GAMEPAD_DPAD_UP)) && timer_ <= 0) {
			if (menuNum_ > 0) {
				menuNum_--;
				timer_ = TIME_MOVE;
			}
		}
		if (input->TriggerButton(XINPUT_GAMEPAD_B)) {
			isSetting_ = false;
		}

	}
}

void Setting::Draw()
{
	//設定画面中のみ表示
	if (isSetting_) {

	}
}
