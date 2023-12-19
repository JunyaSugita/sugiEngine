#include "Setting.h"
#include "Input.h"

Setting* Setting::GetInstance()
{
	static Setting instance;

	return &instance;
}

void Setting::Initialize()
{
	isActive_ = true;
	menuNum_ = 0;
	timer_ = 0;

	sprite_[BGM].Initialize(Sprite::LoadTexture("BGMVol.png"));
	sprite_[BGM].SetPos({300,100});
	sprite_[SE].Initialize(Sprite::LoadTexture("SEVol.png"));
	sprite_[SE].SetPos({ 300,200 });
	sprite_[XSensi].Initialize(Sprite::LoadTexture("XSensi.png"));
	sprite_[XSensi].SetPos({ 300,450 });
	sprite_[YSensi].Initialize(Sprite::LoadTexture("YSensi.png"));
	sprite_[YSensi].SetPos({ 300,550 });
	slider_[BGM].Initialize({ 800.0f,100.0f }, 0.0f, 1.0f, 500.0f);
	slider_[SE].Initialize({ 800.0f,200.0f }, 0.0f, 1.0f, 500.0f);
	slider_[XSensi].Initialize({ 800.0f,450.0f }, 0.1f, 2.0f, 500.0f);
	slider_[YSensi].Initialize({ 800.0f,550.0f }, 0.1f, 2.0f, 500.0f);

	for (int i = 0;i < 4;i++) {
		sprite_[i].SetAnchorPoint(0.5f, 0.5f);
	}
}

void Setting::Update()
{
	//設定画面中のみ操作可能
	if (isActive_) {

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
			isActive_ = false;
		}

		//設定項目の変更
		if ((input->GetLStickX() < -10000 || input->PushButton(XINPUT_GAMEPAD_DPAD_LEFT)) && timer_ <= 0) {
			switch (menuNum_)
			{
			case BGM:
				BGMVol_ -= 0.01f;
				break;
			case SE:
				SEVol_ -= 0.01f;
				break;
			case XSensi:
				XSensi_ -= 0.02f;
				break;
			case YSensi:
				YSensi_ -= 0.02f;
				break;
			default:
				break;
			}
		}
		if ((input->GetLStickX() > 10000 || input->PushButton(XINPUT_GAMEPAD_DPAD_RIGHT)) && timer_ <= 0) {
			switch (menuNum_)
			{
			case BGM:
				BGMVol_ += 0.01f;
				break;
			case SE:
				SEVol_ += 0.01f;
				break;
			case XSensi:
				XSensi_ += 0.02f;
				break;
			case YSensi:
				YSensi_ += 0.02f;
				break;
			default:
				break;
			}
		}


		slider_[BGM].Update(BGMVol_);
		slider_[SE].Update(SEVol_);
		slider_[XSensi].Update(XSensi_);
		slider_[YSensi].Update(YSensi_);

		for (int i = 0;i < 4;i++) {
			if (i == menuNum_) {
				slider_[i].SetPointColor({ 0.8f,0.3f,0.3f });
			}else{
				slider_[i].SetPointColor({0.8f,0.8f,0.8f});
			}
		}
	}
}

void Setting::Draw()
{
	//設定画面中のみ表示
	if (isActive_) {
		for (int i = 0; i < 4; i++) {
			sprite_[i].Draw();
			slider_[i].Draw();
		}
	}
}
