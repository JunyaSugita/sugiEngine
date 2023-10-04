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
#include "LoadOut.h"
#include "NavePointManager.h"
#include "ClearChecker.h"
#include "StageSelectManager.h"
#include "MenuManager.h"

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
	
	//クリアの判定
	ClearChecker::GetInstance()->Initialize();
	//グラウンド
	stageNum_ = StageSelectManager::GetInstance()->GetSelectNum();
	FieldManager::GetInstance()->Initialize(stageNum_);

	//プレイヤー
	Player::GetInstance()->Initialize();

	if (stageNum_ == TUTORIAL || stageNum_ == SET_SPELL_STAGE) {
		BaseEnemy::SetIsAllStop(true);
		if (stageNum_ == TUTORIAL) {
			Tutorial::GetInstance()->SetIsTutorial(true);
		}
		else {
			Tutorial::GetInstance()->SetIsTutorial(false);
		}
	}
	else {
		Enemy::SetIsAllStop(false);
		Tutorial::GetInstance()->SetIsTutorial(false);
	}

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

	gameOver_.Initialize();

	sound_.Initialize();
	sound_.LoadWave("mainBGM");
	sound_.RePlayWave("mainBGM", true);
	sound_.SetVolume("mainBGM", 0.1f);

	LoadOut::GetInstance()->Initialize();
	MenuManager::GetInstance()->Initialize();
}

void GameScene::GameInitialize()
{
	EnemyManager::GetInstance()->GameInitialize();
	FieldManager::GetInstance()->Initialize(stageNum_);
	Player::GetInstance()->GameInitialize();

	ClearChecker::GetInstance()->GameInitialize();
	gameOver_.Initialize();
	UIManager::GetInstance()->GameInitialize();
	sound_.RePlayWave("mainBGM", true);
	sound_.SetVolume("mainBGM", 0.1f);
	MenuManager::GetInstance()->GameInitialize();
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
	LoadOut* loadOut = LoadOut::GetInstance();

#pragma endregion

#pragma region デバッグ用
#ifdef _DEBUG
	if (!particleE_->GetIsEdit(0)) {

		if (input->TriggerKey(DIK_P)) {
			ParticleManager::GetInstance()->Clear();
			GameManager::GetInstance()->SetTitleScene();
		}
		if (input->TriggerKey(DIK_R)) {
			Initialize();
		}
		if (input->TriggerButton(XINPUT_GAMEPAD_X)) {
			enemyM->PopEnemy({ -10,0,0 });
			enemyM->PopEnemy({ -5,0,3 });
			enemyM->PopEnemy({ 0,0,0 });
			enemyM->PopEnemy({ 5,0,3 });
			enemyM->PopEnemy({ 10,0,0 });
			ClearChecker::GetInstance()->Initialize();
		}
	}
#endif
#pragma endregion

	if (MenuManager::GetInstance()->GetIsMenu()) {
		MenuManager::GetInstance()->Update();

		if (MenuManager::GetInstance()->GetIsReset()) {
			GameInitialize();
		}

		//シーン切り替え系
		if (MenuManager::GetInstance()->GetIsStageSelect()) {
			GameManager::GetInstance()->SetStageSelectScene();
		}
		return;
	}

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
	loadOut->Update();
	MenuManager::GetInstance()->Update();

#pragma endregion

#pragma region ImGui
#ifdef _DEBUG
	{
		Begin("EnemyDebug");
		if (Button("stop", { 150,30 })) {
			Enemy::ToggleIsAllStop();
		}
		End();

		Begin("DebugButton");
		if (Button("Reset", { 150,30 })) {
			Initialize();
		}
		if (Button("EnemyPop", { 150,30 })) {
			enemyM->PopEnemy({0,0,0});
		}
		if (Button("SlimePop", { 150,30 })) {
			enemyM->PopSlime({ 0,0,0 });
		}
		if (Button("ShowHitBox", { 150,30 })) {
			colM->ChangeIsShowHitBox();
		}
		if (Button("particleClear", { 150,30 })) {
			ParticleManager::GetInstance()->Clear();
		}
		if (Button("PlayerInvincible", { 150,30 })) {
			Player::GetInstance()->SetInvincible();
		}
		End();
	}

#endif
#pragma endregion

	if (Tutorial::GetInstance()->GetIsTutorial() && EnemyManager::GetInstance()->GetEnemyCount() <= 0) {
		enemyM->PopEnemy({ 0,0,15 });
	}

	ClearChecker::GetInstance()->Update();
	gameOver_.Update();

	//シーン遷移処理
	if (StageSelectManager::GetInstance()->GetSelectNum() == SET_SPELL_STAGE && input->TriggerButton(XINPUT_GAMEPAD_Y)) {
		loadOut->ToggleIsActive();
	}

	if (input->TriggerButton(XINPUT_GAMEPAD_START) || input->TriggerKey(DIK_ESCAPE)) {
		MenuManager::GetInstance()->SetGameMenu();
	}

	//デバッグ関連
	if (stageNum_ == SET_SPELL_STAGE) {
		if (enemyM->GetEnemyCount() <= 0) {
			enemyM->PopEnemy({0,0,15});
		}
	}

	//Initialize系
	if (UIManager::GetInstance()->GetStateAlpha_() != 0 && (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_Z))) {
		if (player->GetLife() > 0) {
			GameManager::GetInstance()->SetStageSelectScene();
			return;
		}
		GameInitialize();
	}
	if (MenuManager::GetInstance()->GetIsReset()) {
		GameInitialize();
	}

	//シーン切り替え系
	if (MenuManager::GetInstance()->GetIsStageSelect()) {
		GameManager::GetInstance()->SetStageSelectScene();
	}
}

void GameScene::BackSpriteDraw()
{
	LoadOut::GetInstance()->BackDrawSp();
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
	ClearChecker::GetInstance()->Draw();
}

void GameScene::ParticleDraw()
{
	ParticleManager::GetInstance()->Draw();
	NavePointManager::GetInstance()->Draw();
}

void GameScene::SpriteDraw()
{
	if (!particleE_->GetIsEdit(0)) {

		if (!LoadOut::GetInstance()->GetIsActive()) {
			Player::GetInstance()->SpDraw();
			gameOver_.Draw();
		}
		UIManager::GetInstance()->Draw();

		MenuManager::GetInstance()->Draw();
	}
}

void GameScene::Finalize()
{
}
