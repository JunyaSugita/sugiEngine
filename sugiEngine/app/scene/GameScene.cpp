#include "GameScene.h"
#include "Input.h"
#include "FbxLoader.h"
#include "PostEffect.h"

#include "Player.h"
#include "EnemyManager.h"
#include "GroundManager.h"
#include "EffectManager.h"

using namespace ImGui;

void GameScene::Initialize()
{
	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());
	Fbx::SetLight(lightGroup_.get());

	//カメラ
	Camera::GetInstance()->SetTarget(Vector3(0, 0, 0));
	Camera::GetInstance()->SetEye(Vector3(0, 1, -10));

	//地面
	GroundManager::GetInstance()->Initialize();

	//プレイヤー
	Player::GetInstance()->Initialize();
	//敵
	EnemyManager::GetInstance()->Initialize();

	//エフェクト
	EffectManager::GetInstance()->Initialize();
}

void GameScene::Update()
{
#pragma region インスタンス呼び出し
	Input* input = Input::GetInstance();
	Player* player = Player::GetInstance();
	EnemyManager* enemyM = EnemyManager::GetInstance();
	GroundManager* groundM = GroundManager::GetInstance();
	EffectManager* effectM = EffectManager::GetInstance();

#pragma endregion

#pragma region ライト
	//ライト
	lightGroup_->Update();
#pragma endregion

#pragma region デバッグ用

	if (input->TriggerKey(DIK_1)) {
		GameManager::GetInstance()->SetTitleScene();
	}

	if (input->TriggerKey(DIK_E)) {
		enemyM->PopEnemy();
	}
#pragma endregion

#pragma region Update呼び出し
	//Update呼び出し
	groundM->Update();
	player->Update();//プレイヤー
	enemyM->Update();//敵
	effectM->Update();
#pragma endregion

#pragma region ImGui
	{
		Begin("PostEffect");
		if (Button("Clear", { 100,30 })) {
			PostEffect::SetClear();
		}
		if (Button("Blur", { 100,30 })) {
			PostEffect::SetBlur();
		}
		if (Button("InvertColor", { 100,30 })) {
			PostEffect::SetInvertColor();
		}
		if (Button("Border", { 100,30 })) {
			PostEffect::SetBorder();
		}
		if (Button("Gray", { 100,30 })) {
			PostEffect::SetGray();
		}
		if (Button("Bloom", { 100,30 })) {
			PostEffect::SetBloom();
		}
		if (Button("Closs4", { 100,30 })) {
			PostEffect::SetCloss4();
		}
		if (Button("Closs6", { 100,30 })) {
			PostEffect::SetCloss6();
		}
		if (Button("Closs8", { 100,30 })) {
			PostEffect::SetCloss8();
		}
		End();

		Begin("PlayerState");
		Text("Life %d", player->GetLife());
		End();
	}

#pragma endregion

}

void GameScene::BackSpriteDraw()
{
}

void GameScene::Draw()
{
}

void GameScene::ObjDraw()
{
	GroundManager::GetInstance()->Draw();
	Player::GetInstance()->Draw();
	EnemyManager::GetInstance()->Draw();
	EffectManager::GetInstance()->Draw();
}

void GameScene::SpriteDraw()
{
}

void GameScene::Finalize()
{
}
