#include "MyGame.h"

using namespace DirectX;
using namespace std;

void MyGame::Initialize()
{
	//���N���X�̏�����
	SugiFramework::Initialize();
	
	//�Q�[���ŗL�̏�����
	ImGuiManager::GetInstance()->Initialize(winApp_.get(), dxCom_.get());
}

void MyGame::Finalize()
{
	//�Q�[���ŗL�̏I������
	GameManager::GetInstance()->Finalize();
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

	postEffect2->PreDrawScene(dxCom_->GetCommandList());
	postEffect->Draw(dxCom_->GetCommandList());
	postEffect2->PostDrawScene(dxCom_->GetCommandList());

	SugiFramework::dxCom_->PreDraw();
	//�w�i�X�v���C�g
	Sprite::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->BackSpriteDraw();
	Sprite::PostDraw();

	postEffect2->Draw(dxCom_->GetCommandList());

	//�X�v���C�g
	Sprite::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->SpriteDraw();
	Sprite::PostDraw();

	ImGuiManager::GetInstance()->Draw();
	SugiFramework::dxCom_->PostDraw();

	//SugiFramework::dxCom_->PreDraw();
	//GameDraw();
	//ImGuiManager::GetInstance()->Draw();
	//SugiFramework::dxCom_->PostDraw();
}

void MyGame::GameDraw()
{
	//�w�i�X�v���C�g
	//Sprite::PreDraw(dxCom_->GetCommandList());
	//GameManager::GetInstance()->BackSpriteDraw();
	//Sprite::PostDraw();

	//Fbx���f��
	Fbx::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->Draw();
	Fbx::PostDraw();

	//obj���f��
	Object3d::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->ObjDraw();
	Object3d::PostDraw();

	//particle���f��
	ParticleManager::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->ParticleDraw();
	ParticleManager::PostDraw();

	Object3d::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->ObjDraw2();
	Object3d::PostDraw();

	//�X�v���C�g
	//Sprite::PreDraw(dxCom_->GetCommandList());
	//GameManager::GetInstance()->SpriteDraw();
	//Sprite::PostDraw();
}
