#include "Input.h"

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
