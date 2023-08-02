#include "Input.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment (lib, "xinput.lib")

Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

void Input::Initialize(WinApp* winApp)
{
	winApp_ = winApp;
	HRESULT result;
	//DirectInput�̏�����
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard_->SetCooperativeLevel(winApp->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard_->Acquire();
	//�O�t���[���̃L�[����oldKey�Ɉڂ�
	memcpy(oldKey_, key_, sizeof(key_));
	//�S�L�[�̓��͏�Ԃ��擾����
	keyboard_->GetDeviceState(sizeof(key_), key_);

	oldState_ = state_;
	ZeroMemory(&state_, sizeof(XINPUT_STATE));
	DWORD dwResult = XInputGetState(0, &state_);

	if (state_.Gamepad.sThumbLX <  DEAD_ZONE && state_.Gamepad.sThumbLX > -DEAD_ZONE) {
		state_.Gamepad.sThumbLX = 0;
	}
	if (state_.Gamepad.sThumbLY <  DEAD_ZONE && state_.Gamepad.sThumbLY > -DEAD_ZONE) {
		state_.Gamepad.sThumbLY = 0;
	}
	if (state_.Gamepad.sThumbRX <  DEAD_ZONE && state_.Gamepad.sThumbRX > -DEAD_ZONE) {
		state_.Gamepad.sThumbRX = 0;
	}
	if (state_.Gamepad.sThumbRY <  DEAD_ZONE && state_.Gamepad.sThumbRY > -DEAD_ZONE) {
		state_.Gamepad.sThumbRY = 0;
	}
}

bool Input::PushKey(int32_t keyNum)
{
	if (key_[keyNum]) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(int32_t keyNum)
{
	if (key_[keyNum] && oldKey_[keyNum] == 0) {
		return true;
	}
	return false;
}

bool Input::ReleaseKey(int32_t keyNum)
{
	if (key_[keyNum] == 0 && oldKey_[keyNum]) {
		return true;
	}
	return false;
}

bool Input::PushButton(int32_t buttonNum)
{
	if (state_.Gamepad.wButtons & buttonNum) {
		return true;
	}

	return false;
}

bool Input::TriggerButton(int32_t buttonNum)
{
	if ((state_.Gamepad.wButtons & buttonNum) && (oldState_.Gamepad.wButtons & buttonNum) == 0) {
		return true;
	}
	return false;
}

bool Input::ReleaseButton(int32_t buttonNum)
{
	if ((state_.Gamepad.wButtons & buttonNum) == 0 && (oldState_.Gamepad.wButtons & buttonNum)) {
		return true;
	}
	return false;
}

SHORT Input::GetLStickX()
{
	return state_.Gamepad.sThumbLX;
}

SHORT Input::GetLStickY()
{
	return state_.Gamepad.sThumbLY;
}

SHORT Input::GetRStickX()
{
	return state_.Gamepad.sThumbRX;
}

SHORT Input::GetRStickY()
{
	return state_.Gamepad.sThumbRY;
}

BYTE Input::GetLTrigger()
{
	return state_.Gamepad.bLeftTrigger;
}

bool Input::TriggerLStickRight()
{
	if (GetLStickX() > 10000) {
		if (!isLSRight_) {
			isLSRight_ = true;
			return true;
		}
	}
	else {
		isLSRight_ = false;
	}
	return false;
}

bool Input::TriggerLStickLeft()
{
	if (GetLStickX() < -10000) {
		if (!isLSLeft_) {
			isLSLeft_ = true;
			return true;
		}
	}
	else {
		isLSLeft_ = false;
	}
	return false;
}

bool Input::TriggerLStickUp()
{
	if (GetLStickY() > 10000) {
		if (!isLSUp_) {
			isLSUp_ = true;
			return true;
		}
	}
	else {
		isLSUp_ = false;
	}
	return false;
}

bool Input::TriggerLStickDown()
{
	if (GetLStickY() < -10000) {
		if (!isLSDown_) {
			isLSDown_ = true;
			return true;
		}
	}
	else {
		isLSDown_ = false;
	}
	return false;
}
