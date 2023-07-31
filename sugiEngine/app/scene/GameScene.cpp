#include "GameScene.h"
#include "Input.h"
#include "FbxLoader.h"
#include "PostEffect.h"

#include "Player.h"
#include "EnemyManager.h"
#include "EffectManager.h"
#include "SpellManager.h"
#include "UIManager.h"
#include "ColliderManager.h"
#include "Fieldmanager.h"
#include "Tutorial.h"

using namespace ImGui;
using namespace std;

void GameScene::Initialize()
{
	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());
	Fbx::SetLight(lightGroup_.get());

	//カメラ
	Camera::GetInstance()->SetTarget(Vector3(0, 0, 0));
	Camera::GetInstance()->SetEye(Vector3(0, 1, -10));

	//敵
	EnemyManager::GetInstance()->Initialize();
	
	//グラウンド
	FieldManager::GetInstance()->Initialize();

	//プレイヤー
	Player::GetInstance()->Initialize();


	//エフェクト
	EffectManager::GetInstance()->Initialize();

	//魔法
	SpellManager::GetInstance()->Initialize();

	//UI
	UIManager::GetInstance()->Initialize();

	//当たり判定
	ColliderManager::GetInstance()->Initialize();

	ParticleManager* particleM = ParticleManager::GetInstance();
	particleM->Initialize();

	particleE_ = make_unique<ParticleEditor>();
	particleE_->Initialize();

	clearChecker_.Initialize();
	gameOver_.Initialize();
}

void GameScene::Update()
{
#pragma region インスタンス呼び出し
	Input* input = Input::GetInstance();
	Player* player = Player::GetInstance();
	EnemyManager* enemyM = EnemyManager::GetInstance();
	FieldManager* fieldM = FieldManager::GetInstance();
	EffectManager* effectM = EffectManager::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();
	UIManager* uiM = UIManager::GetInstance();
	ColliderManager* colM = ColliderManager::GetInstance();
	ParticleManager* particleM = ParticleManager::GetInstance();

#pragma endregion

#pragma region デバッグ用
	if (!particleE_->GetIsEdit(0)) {

		if (input->TriggerKey(DIK_P)) {
			ParticleManager::GetInstance()->Clear();
			GameManager::GetInstance()->SetTitleScene();
		}
		if (input->TriggerKey(DIK_R)) {
			Initialize();
		}
		if (input->TriggerButton(XINPUT_GAMEPAD_Y)) {
			player->SubLife();
			clearChecker_.Initialize();
		}
		if (input->TriggerButton(XINPUT_GAMEPAD_X)) {
			enemyM->PopEnemy({ -10,0,0 });
			enemyM->PopEnemy({ -5,0,3 });
			enemyM->PopEnemy({ 0,0,0 });
			enemyM->PopEnemy({ 5,0,3 });
			enemyM->PopEnemy({ 10,0,0 });
			clearChecker_.Initialize();
		}

		//if (input->TriggerKey(DIK_O)) {
		//	colM->ChangeIsShowHitBox();
		//}
		//if (input->TriggerKey(DIK_1)) {
		//	player->SetPresetSpell(FIRE_BALL);
		//}
		//if (input->TriggerKey(DIK_2)) {
		//	player->SetPresetSpell(MAGIC_MISSILE);
		//}
		//if (input->TriggerKey(DIK_3)) {
		//	player->SetPresetSpell(ICE_BOLT);
		//}
	}
#pragma endregion

#pragma region Update呼び出し
	//Update呼び出し
	lightGroup_->Update();
	fieldM->Update();
	player->Update();//プレイヤー
	enemyM->Update();//敵
	effectM->Update();
	spellM->Update();
	uiM->Update();
	colM->Update();
	particleM->Update();
	particleE_->Update();

#pragma endregion

#pragma region ImGui
#ifdef _DEBUG
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

		Begin("EnemyDebug");
		if (Button("stop", { 150,30 })) {
			Enemy::SetIsDebugStop();
		}
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
		if (Button("particleClear", { 150,30 })) {
			ParticleManager::GetInstance()->Clear();
		}
		End();
	}

#endif
#pragma endregion

	if (Tutorial::GetInstance()->GetIsTutorial() && EnemyManager::GetInstance()->GetEnemyCount() <= 0) {
		enemyM->PopEnemy({ -10,0,0 });
		enemyM->PopEnemy({ -5,0,3 });
		enemyM->PopEnemy({ 0,0,0 });
		enemyM->PopEnemy({ 5,0,3 });
		enemyM->PopEnemy({ 10,0,0 });
		clearChecker_.Initialize();
	}

	clearChecker_.Update();
	gameOver_.Update();

	//シーン遷移処理
	if (Tutorial::GetInstance()->GetIsTutorial() && Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_A)) {
		Tutorial::GetInstance()->SetIsTutorial(false);
		GameManager::GetInstance()->SetGameScene();
	}

	if (UIManager::GetInstance()->GetStateAlpha_() != 0 && Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_A)) {
		if (player->GetLife() > 0) {
			GameManager::GetInstance()->SetTitleScene();
		}
		else {
			GameManager::GetInstance()->SetGameScene();
		}
	}
}

void GameScene::BackSpriteDraw()
{
}

void GameScene::Draw()
{
}

void GameScene::ObjDraw()
{
	if (!particleE_->GetIsEdit(0)) {
		FieldManager::GetInstance()->Draw();

		EnemyManager::GetInstance()->Draw();
		EffectManager::GetInstance()->Draw();
		SpellManager::GetInstance()->Draw();
	}
}

void GameScene::ObjDraw2()
{
	if (!particleE_->GetIsEdit(0)) {
		Player::GetInstance()->Draw();
	}
}

void GameScene::ParticleDraw()
{
	ParticleManager::GetInstance()->Draw();
}

void GameScene::SpriteDraw()
{
	if (!particleE_->GetIsEdit(0)) {

		Player::GetInstance()->SpDraw();
		clearChecker_.Draw();
		gameOver_.Draw();
		UIManager::GetInstance()->Draw();
	}
}

void GameScene::Finalize()
{
}
