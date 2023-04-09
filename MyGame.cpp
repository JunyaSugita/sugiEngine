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

	

#pragma region windowsAPI初期化処理

	winApp->CreateWindowScreen();
	//コンソールへ文字入力
	OutputDebugStringA("Hello DirectX\n");
	OutputDebugStringA("This is sugiEngine\n");

#pragma endregion

#pragma region DirectX初期化処理
	// DirectX初期化処理 ここから
	dxCom->Initialize(winApp.get());
	//キーボード入力の初期化
	input->Initialize(winApp.get());



#pragma endregion

#pragma region 描画初期化処理

	Object3d::StaticInitialize(dxCom->GetDevice());
	Sprite::StaticInitialize(dxCom->GetDevice());
	LightGroup::StaticInitialize(dxCom->GetDevice());
	imGuiManager->Initialie(winApp.get(), dxCom.get());

#pragma endregion

	gameM = make_unique <GameManager>();

}

void MyGame::Finalize()
{

#pragma region WindowsAPI後始末
	winApp->DeleteWindow();

#pragma endregion

#pragma region delete処理 
	gameM->Delete();
	imGuiManager->Finalize();
#pragma endregion

}

void MyGame::Update()
{
	//キーボード入力
	input->Update();

#pragma region ゲームシーン

	///
	//アップデート
	///
	imGuiManager->Begin();
	gameM->Update(input.get(), imGuiManager.get());
	imGuiManager->End();
}

void MyGame::Draw()
{
	dxCom->PreDraw();

	//背景スプライト
	Sprite::PreDraw(dxCom->GetCommandList());

	gameM->BackSpriteDraw();

	Sprite::PostDraw();

	//モデル
	Object3d::PreDraw(dxCom->GetCommandList());

	gameM->Draw();

	Object3d::PostDraw();

	//スプライト
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
