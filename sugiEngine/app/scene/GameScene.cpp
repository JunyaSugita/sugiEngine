#include "GameScene.h"
#include "Input.h"
#include "FbxLoader.h"
#include "PostEffect.h"

#include "Player.h"
#include "EnemyManager.h"
#include "GroundManager.h"
#include "EffectManager.h"
#include "SpellManager.h"
#include "UIManager.h"
#include "ColliderManager.h"

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

	//魔法
	SpellManager::GetInstance()->Initialize();

	//UI
	UIManager::GetInstance()->Initialize();

	//当たり判定
	ColliderManager::GetInstance()->Initialize();

}

void GameScene::Update()
{
#pragma region インスタンス呼び出し
	Input* input = Input::GetInstance();
	Player* player = Player::GetInstance();
	EnemyManager* enemyM = EnemyManager::GetInstance();
	GroundManager* groundM = GroundManager::GetInstance();
	EffectManager* effectM = EffectManager::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();
	UIManager* uiM = UIManager::GetInstance();
	ColliderManager* colM = ColliderManager::GetInstance();

#pragma endregion

#pragma region デバッグ用

	//if (input->TriggerKey(DIK_1)) {
	//	GameManager::GetInstance()->SetTitleScene();
	//}

	if (input->TriggerKey(DIK_R)) {
		Initialize();
	}
	if (input->TriggerKey(DIK_P)) {
		enemyM->PopEnemy();
	}
	if (input->TriggerKey(DIK_O)) {
		colM->ChangeIsShowHitBox();
	}
	if (input->TriggerKey(DIK_1)) {
		player->SetPresetSpell(FIRE_BALL);
	}
	if (input->TriggerKey(DIK_2)) {
		player->SetPresetSpell(MAGIC_MISSILE);
	}

#pragma endregion

#pragma region Update呼び出し
	//Update呼び出し
	lightGroup_->Update();
	groundM->Update();
	player->Update();//プレイヤー
	enemyM->Update();//敵
	effectM->Update();
	spellM->Update();
	uiM->Update();
	colM->Update();

	if (enemyM->GetEnemyCount() < 5) {
		enemyM->PopEnemy();
	}

#pragma endregion

#pragma region ImGui
	{
		//Begin("PostEffect");
		//if (Button("Clear", { 100,30 })) {
		//	PostEffect::SetClear();
		//}
		//if (Button("Blur", { 100,30 })) {
		//	PostEffect::SetBlur();
		//}
		//if (Button("InvertColor", { 100,30 })) {
		//	PostEffect::SetInvertColor();
		//}
		//if (Button("Border", { 100,30 })) {
		//	PostEffect::SetBorder();
		//}
		//if (Button("Gray", { 100,30 })) {
		//	PostEffect::SetGray();
		//}
		//if (Button("Bloom", { 100,30 })) {
		//	PostEffect::SetBloom();
		//}
		//if (Button("Closs4", { 100,30 })) {
		//	PostEffect::SetCloss4();
		//}
		//if (Button("Closs6", { 100,30 })) {
		//	PostEffect::SetCloss6();
		//}
		//if (Button("Closs8", { 100,30 })) {
		//	PostEffect::SetCloss8();
		//}
		//End();

		Begin("PlayerState");
		Text("Life %d", player->GetLife());
		End();

		Begin("DebugButton");
		if (Button("Reset", { 150,30 })) {
			Initialize();
		}
		if (Button("EnemyPop", { 150,30 })) {
			enemyM->PopEnemy();
		}
		if (Button("ShowHitBox", { 150,30 })) {
			colM->ChangeIsShowHitBox();
		}
		if (Button("SetSpell FireBall", { 150,30 })) {
			player->SetPresetSpell(FIRE_BALL);
		}
		if (Button("SetSpell MagicMissile", { 150,30 })) {
			player->SetPresetSpell(MAGIC_MISSILE);
		}
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
	SpellManager::GetInstance()->Draw();
}

void GameScene::SpriteDraw()
{
	UIManager::GetInstance()->Draw();
}

void GameScene::Finalize()
{
}
