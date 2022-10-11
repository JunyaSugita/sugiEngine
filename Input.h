#pragma once
#define DIRECTINPUT_VERSION	0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <assert.h>
#include <wrl.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input {
public:
	void Initialize(WNDCLASSEX w, HWND hwnd);
	void Update();

	bool PushKey(int keyNum);
	bool TriggerKey(int keyNum);
	bool ReleaseKey(int keyNum);
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ComPtr<IDirectInput8> directInput;
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
	BYTE oldKey[256] = {};
};

