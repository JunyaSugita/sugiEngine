#include "Input.h"

void Input::Initialize(HRESULT result, WNDCLASSEX w, HWND hwnd)
{
	//DirectInput�̏�����
	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	//�O�t���[���̃L�[����oldKey�Ɉڂ�
	for (int i = 0; i < 256; i++) {
		oldKey[i] = key[i];
	}
	//�S�L�[�̓��͏�Ԃ��擾����
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
