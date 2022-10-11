#include "Input.h"

void Input::Initialize(WinApp* winApp)
{
	winApp_ = winApp;
	HRESULT result;
	//DirectInputの初期化
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(winApp->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//キーボード情報の取得開始
	keyboard_->Acquire();
	//前フレームのキー情報をoldKeyに移す
	memcpy(oldKey_, key_, sizeof(key_));
	//全キーの入力状態を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);
}

bool Input::PushKey(int keyNum)
{
	if (key_[keyNum]) {
		return true;
	}
	else {
		return false;
	}
}

bool Input::TriggerKey(int keyNum)
{
	if (key_[keyNum] && oldKey_[keyNum] == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Input::ReleaseKey(int keyNum)
{
	if (key_[keyNum] == 0 && oldKey_[keyNum]) {
		return true;
	}
	else {
		return false;
	}
}
