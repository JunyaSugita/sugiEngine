#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

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
	//DWORD dwResult = XInputGetState(0, &state_);

	if (state_.Gamepad.sThumbLX <  2000 && state_.Gamepad.sThumbLX > -2000) {
		state_.Gamepad.sThumbLX = 0;
	}
	if (state_.Gamepad.sThumbLY <  2000 && state_.Gamepad.sThumbLY > -2000) {
		state_.Gamepad.sThumbLY = 0;
	}
}

bool Input::PushKey(uint32_t keyNum)
{
	if (key_[keyNum]) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(uint32_t keyNum)
{
	if (key_[keyNum] && oldKey_[keyNum] == 0) {
		return true;
	}
	return false;
}

bool Input::ReleaseKey(uint32_t keyNum)
{
	if (key_[keyNum] == 0 && oldKey_[keyNum]) {
		return true;
	}
	return false;
}

bool Input::PushButton(uint32_t buttonNum)
{
	if (state_.Gamepad.wButtons & buttonNum) {
		return true;
	}

	return false;
}

bool Input::TriggerButton(uint32_t buttonNum)
{
	if ((state_.Gamepad.wButtons & buttonNum) && (oldState_.Gamepad.wButtons & buttonNum) == 0) {
		return true;
	}
	return false;
}

bool Input::ReleaseButton(uint32_t buttonNum)
{
	if ((state_.Gamepad.wButtons & buttonNum) == 0 && (oldState_.Gamepad.wButtons & buttonNum)) {
		return true;
	}
	return false;
}

SHORT Input::GetLSteckX()
{
	return state_.Gamepad.sThumbLX;
}

SHORT Input::GetLSteckY()
{
	return state_.Gamepad.sThumbLY;
}

BYTE Input::GetLTrigger()
{
	return state_.Gamepad.bLeftTrigger;
}
