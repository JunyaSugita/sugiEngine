#include "MyGame.h"

using namespace DirectX;
using namespace std;

void MyGame::Initialize()
{
	winApp = make_unique<WinApp>();
	dxCom = make_unique<DXCommon>();
	input = make_unique<Input>();
	matrix4 = make_unique <Matrix4>();
	imGuiManager = make_unique <ImGuiManager>();

	

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
	input->Initialize(winApp.get());



#pragma endregion

#pragma region �`�揉��������

	Object3d::StaticInitialize(dxCom->GetDevice());
	Sprite::StaticInitialize(dxCom->GetDevice());
	LightGroup::StaticInitialize(dxCom->GetDevice());
	imGuiManager->Initialie(winApp.get(), dxCom.get());

#pragma endregion

	gameM = make_unique <GameManager>();

}

void MyGame::Finalize()
{

#pragma region WindowsAPI��n��
	winApp->DeleteWindow();

#pragma endregion

#pragma region delete���� 
	gameM->Delete();
	imGuiManager->Finalize();
#pragma endregion

}

void MyGame::Update()
{
	//�L�[�{�[�h����
	input->Update();

#pragma region �Q�[���V�[��

	///
	//�A�b�v�f�[�g
	///
	imGuiManager->Begin();
	gameM->Update(input.get(), imGuiManager.get());
	imGuiManager->End();
}

void MyGame::Draw()
{
	dxCom->PreDraw();

	//�w�i�X�v���C�g
	Sprite::PreDraw(dxCom->GetCommandList());

	gameM->BackSpriteDraw();

	Sprite::PostDraw();

	//���f��
	Object3d::PreDraw(dxCom->GetCommandList());

	gameM->Draw();

	Object3d::PostDraw();

	//�X�v���C�g
	Sprite::PreDraw(dxCom->GetCommandList());

	gameM->SpriteDraw();

	Sprite::PostDraw();

	imGuiManager->Draw();


	dxCom->PostDraw();
}

bool MyGame::GetIsEnd()
{
	if (winApp->ProcMessage()) {
		return true;
	}
	return false;
}
