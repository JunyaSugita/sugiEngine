#include "MyGame.h"

using namespace DirectX;
using namespace std;

void MyGame::Initialize()
{
	//���N���X�̏�����
	SugiFramework::Initialize();
	
	//�Q�[���ŗL�̏�����
	gameM = make_unique <GameManager>();
	imGuiManager = make_unique <ImGuiManager>();

	imGuiManager->Initialie(winApp.get(), dxCom.get());
}

void MyGame::Finalize()
{
	//�Q�[���ŗL�̏I������
	gameM->Delete();
	//���N���X�̏I������
	SugiFramework::Finalize();
	imGuiManager->Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	SugiFramework::Update();

	//�Q�[���ŗL�̍X�V����
	imGuiManager->Begin();
	gameM->Update(input.get(), imGuiManager.get());
	imGuiManager->End();
}

void MyGame::Draw()
{
	SugiFramework::dxCom->PreDraw();

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
