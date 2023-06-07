#include "MyGame.h"

using namespace DirectX;
using namespace std;

void MyGame::Initialize()
{
	//���N���X�̏�����
	SugiFramework::Initialize();
	
	//�Q�[���ŗL�̏�����
	ImGuiManager::GetInstance()->Initialie(winApp.get(), dxCom.get());
}

void MyGame::Finalize()
{
	//�Q�[���ŗL�̏I������
	GameManager::GetInstance()->Delete();
	//���N���X�̏I������
	SugiFramework::Finalize();
	ImGuiManager::GetInstance()->Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	SugiFramework::Update();

	//�Q�[���ŗL�̍X�V����
	ImGuiManager::GetInstance()->Begin();
	GameManager::GetInstance()->Update();
	ImGuiManager::GetInstance()->End();

	//�J�����̈ړ�
	Camera::GetInstance()->Update();
}

void MyGame::Draw()
{
	SugiFramework::dxCom->PreDraw();

	//�w�i�X�v���C�g
	Sprite::PreDraw(dxCom->GetCommandList());

	GameManager::GetInstance()->BackSpriteDraw();

	Sprite::PostDraw();

	//Fbx���f��
	Fbx::PreDraw(dxCom->GetCommandList());
	GameManager::GetInstance()->Draw();
	Fbx::PostDraw();

	//obj���f��
	Object3d::PreDraw(dxCom->GetCommandList());
	GameManager::GetInstance()->ObjDraw();
	Object3d::PostDraw();

	//�X�v���C�g
	Sprite::PreDraw(dxCom->GetCommandList());

	GameManager::GetInstance()->SpriteDraw();

	Sprite::PostDraw();

	ImGuiManager::GetInstance()->Draw();


	dxCom->PostDraw();
}
