#pragma once
#define DIRECTINPUT_VERSION	0x0800	//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include <assert.h>
#include <wrl.h>
#include <Xinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment (lib, "xinput.lib")

#include "WinApp.h"

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689



class Input final{
private:
	Input();
	~Input();

public:
	//�R�s�[�R���X�g���N�^����
	Input(const Input& obj) = delete;
	//������Z�q�𖳌�
	Input& operator=(const Input& obj) = delete;

	static Input* GetInstance();


public:
	void Initialize(WinApp* winApp);
	void Update();

	bool PushKey(int keyNum);
	bool TriggerKey(int keyNum);
	bool ReleaseKey(int keyNum);

	bool PushButton(int buttonNum);
	bool TriggerButton(int buttonNum);
	bool ReleaseButton(int buttonNum);

	SHORT GetLSteckX();
	SHORT GetLSteckY();
	BYTE GetLTrigger();
public:
	//�G�C���A�X�e���v���[�g
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
