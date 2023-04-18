#include "SugiFramework.h"

void SugiFramework::Initialize()
{
	winApp = make_unique<WinApp>();
	dxCom = make_unique<DXCommon>();
	matrix4 = make_unique <Matrix4>();

#pragma region windowsAPI����������
	winApp->CreateWindowScreen();
	//�R���\�[���֕�������
	OutputDebugStringA("Hello DirectX\n");
	OutputDebugStringA("This is sugiEngine\n");
#pragma endregion

#pragma region DirectX����������
	// DirectX���������� ��������
	dxCom->Initialize(winApp.get());
	//�L�[�{�[�h���͂̏�����
	Input::GetInstance()->Initialize(winApp.get());
#pragma endregion

#pragma region �`�揉��������
	Object3d::StaticInitialize(dxCom->GetDevice());
	Sprite::StaticInitialize(dxCom->GetDevice());
	LightGroup::StaticInitialize(dxCom->GetDevice());
#pragma endregion
}

void SugiFramework::Finalize()
{
#pragma region WindowsAPI��n��
	winApp->DeleteWindow();
#pragma endregion
}

void SugiFramework::Update()
{
	//�L�[�{�[�h����
	Input::GetInstance()->Update();
}

void SugiFramework::Run()
{
	//�Q�[��������
	Initialize();

	while (true)
	{
		//���t���[���X�V
		Update();
		//�I�����N�G�X�g���擾
		if (GetIsEnd()) {
			break;
		}
		//�`��
		Draw();
	}

	//�Q�[���̏I��
	Finalize();
}

bool SugiFramework::GetIsEnd()
{
	if (winApp->ProcMessage()) {
		return true;
	}
	return false;
}