#include "MyGame.h"

using namespace DirectX;
using namespace std;

void MyGame::Initialize()
{
	//���N���X�̏�����
	SugiFramework::Initialize();
	
	//�Q�[���ŗL�̏�����
	ImGuiManager::GetInstance()->Initialie(winApp_.get(), dxCom_.get());
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
	postEffect->PreDrawScene(dxCom_->GetCommandList());
	GameDraw();
	postEffect->PostDrawScene(dxCom_->GetCommandList());

	SugiFramework::dxCom_->PreDraw();
	postEffect->Draw(dxCom_->GetCommandList());
	//ImGuiManager::GetInstance()->Draw();
	SugiFramework::dxCom_->PostDraw();
}

void MyGame::GameDraw()
{
	//�w�i�X�v���C�g
	Sprite::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->BackSpriteDraw();
	Sprite::PostDraw();

	//Fbx���f��
	Fbx::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->Draw();
	Fbx::PostDraw();

	//obj���f��
	Object3d::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->ObjDraw();
	Object3d::PostDraw();

	//�X�v���C�g
	Sprite::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->SpriteDraw();
	postEffect->Draw(dxCom_->GetCommandList());
	Sprite::PostDraw();
}
