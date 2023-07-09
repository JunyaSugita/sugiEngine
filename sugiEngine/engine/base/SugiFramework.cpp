#include "SugiFramework.h"

void SugiFramework::Initialize()
{
	winApp_ = make_unique<WinApp>();
	dxCom_ = make_unique<DXCommon>();
	matrix4_ = make_unique <Matrix4>();

#pragma region windowsAPI����������
	winApp_->CreateWindowScreen();
	//�R���\�[���֕�������
	OutputDebugStringA("Hello DirectX\n");
	OutputDebugStringA("This is sugiEngine\n");
#pragma endregion

#pragma region DirectX����������
	// DirectX���������� ��������
	dxCom_->Initialize(winApp_.get());
	//�L�[�{�[�h���͂̏�����
	Input::GetInstance()->Initialize(winApp_.get());
	//FbxLoder������
	FbxLoader::GetInstance()->Initialize(dxCom_->GetDevice());

#pragma endregion

#pragma region �`�揉��������
	Camera::GetInstance()->Initialize();
	Object3d::StaticInitialize(dxCom_->GetDevice());
	Particle::StaticInitialize(dxCom_->GetDevice());
	Fbx::SetDevice(dxCom_->GetDevice());
	Fbx::CreateGraphicsPipeline();
	Sprite::StaticInitialize(dxCom_->GetDevice());
	LightGroup::StaticInitialize(dxCom_->GetDevice());
#pragma endregion

	//PostEffect
	postEffect = make_unique <PostEffect>();
	uint32_t postNum = Sprite::LoadTexture("white1x1.png");

	postEffect->Initialize(dxCom_->GetDevice());
}

void SugiFramework::Finalize()
{
	//�������
	FbxLoader::GetInstance()->Finalize();

#pragma region WindowsAPI��n��
	//�Ō�ɂ���
	winApp_->DeleteWindow();
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
	if (winApp_->ProcMessage()) {
		return true;
	}
	return false;
}