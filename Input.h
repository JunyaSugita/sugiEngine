#pragma once
#define DIRECTINPUT_VERSION	0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <assert.h>
#include <wrl.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"

class Input {
public:
	void Initialize(WinApp* winApp);
	void Update();

	bool PushKey(int keyNum);
	bool TriggerKey(int keyNum);
	bool ReleaseKey(int keyNum);
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<IDirectInput8> directInput_;
	ComPtr<IDirectInputDevice8> keyboard_;
	BYTE key_[256] = {};
	BYTE oldKey_[256] = {};
	WinApp* winApp_ = nullptr;
};

