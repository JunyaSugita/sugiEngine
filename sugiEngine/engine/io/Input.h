#pragma once
#define DIRECTINPUT_VERSION	0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <assert.h>
#include <wrl.h>
#include <Xinput.h>



#include "WinApp.h"

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689



class Input final{
private:
	Input() = default;
	~Input() = default;

public:
	//コピーコンストラクタ無効
	Input(const Input& obj) = delete;
	//代入演算子を無効
	Input& operator=(const Input& obj) = delete;

	static Input* GetInstance();


public:
	void Initialize(WinApp* winApp);
	void Update();

	bool PushKey(int32_t keyNum);
	bool TriggerKey(int32_t keyNum);
	bool ReleaseKey(int32_t keyNum);

	bool PushButton(int32_t buttonNum);
	bool TriggerButton(int32_t buttonNum);
	bool ReleaseButton(int32_t buttonNum);

	SHORT GetLSteckX();
	SHORT GetLSteckY();
	SHORT GetRSteckX();
	SHORT GetRSteckY();
	BYTE GetLTrigger();
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<IDirectInput8> directInput_;
	ComPtr<IDirectInputDevice8> keyboard_;
	BYTE key_[256] = {};
	BYTE oldKey_[256] = {};

	WinApp* winApp_ = nullptr;

	XINPUT_STATE state_ = {};
	XINPUT_STATE oldState_ = {};
};

