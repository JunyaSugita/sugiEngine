#include "MyGame.h"

using namespace DirectX;
using namespace std;

void MyGame::Initialize()
{
	//基底クラスの初期化
	SugiFramework::Initialize();
	
	//ゲーム固有の初期化
	ImGuiManager::GetInstance()->Initialize(winApp_.get(), dxCom_.get());
}

void MyGame::Finalize()
{
	//ゲーム固有の終了処理
	GameManager::GetInstance()->Finalize();
	//基底クラスの終了処理
	SugiFramework::Finalize();
	ImGuiManager::GetInstance()->Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	SugiFramework::Update();

	//ゲーム固有の更新処理
	ImGuiManager::GetInstance()->Begin();
	GameManager::GetInstance()->Update();
	ImGuiManager::GetInstance()->End();

	//カメラの移動
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
	//背景スプライト
	Sprite::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->BackSpriteDraw();
	Sprite::PostDraw();

	postEffect2->Draw(dxCom_->GetCommandList());

	//スプライト
	Sprite::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->SpriteDraw();
	Sprite::PostDraw();

	ImGuiManager::GetInstance()->Draw();
	SugiFramework::dxCom_->PostDraw();
}

void MyGame::GameDraw()
{
	//objモデル
	Object3d::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->ObjDraw();
	Object3d::PostDraw();

	//particleモデル
	Particle::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->ParticleDraw();
	Particle::PostDraw();

	Object3d::PreDraw(dxCom_->GetCommandList());
	GameManager::GetInstance()->DrawTransparent();
	Object3d::PostDraw();
}
