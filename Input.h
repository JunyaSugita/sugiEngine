#pragma once
#define DIRECTINPUT_VERSION	0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <assert.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input{
public:
	void Initialize(HRESULT result, WNDCLASSEX w,HWND hwnd);
	void Update();

	bool PushKey(int keyNum);
	bool TriggerKey(int keyNum);
	bool ReleaseKey(int keyNum);
public:
	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;
	BYTE key[256] = {};
	BYTE oldKey[256] = {};
};

