#include "MyGame.h"

using namespace DirectX;
using namespace std;

void MyGame::Initialize()
{
	//基底クラスの初期化
	SugiFramework::Initialize();
	
	//ゲーム固有の初期化
	gameM = make_unique <GameManager>();
	imGuiManager = make_unique <ImGuiManager>();

	imGuiManager->Initialie(winApp.get(), dxCom.get());
}

void MyGame::Finalize()
{
	//ゲーム固有の終了処理
	gameM->Delete();
	//基底クラスの終了処理
	SugiFramework::Finalize();
	imGuiManager->Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	SugiFramework::Update();

	//ゲーム固有の更新処理
	imGuiManager->Begin();
	gameM->Update(input.get(), imGuiManager.get());
	imGuiManager->End();
}

void MyGame::Draw()
{
	SugiFramework::dxCom->PreDraw();

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
