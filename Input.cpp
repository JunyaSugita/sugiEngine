#include "Input.h"

void Input::Initialize(HRESULT result, WNDCLASSEX w, HWND hwnd)
{
	//DirectInputの初期化
	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//キーボード情報の取得開始
	keyboard->Acquire();
	//前フレームのキー情報をoldKeyに移す
	for (int i = 0; i < 256; i++) {
		oldKey[i] = key[i];
	}
	//全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(int keyNum)
{
	if (key[keyNum]) {
		return true;
	}
	else {
		return false;
	}
}

bool Input::TriggerKey(int keyNum)
{
	if (key[keyNum] && oldKey[keyNum] == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Input::ReleaseKey(int keyNum)
{
	if (key[keyNum] == 0 && oldKey[keyNum]) {
		return true;
	}
	else {
		return false;
	}
}
