#include "GameScene.h"
#include "Input.h"
#include "PostEffect.h"
#include "Player.h"
#include "EnemyManager.h"
#include "EffectManager.h"
#include "SpellManager.h"
#include "UIManager.h"
#include "Fieldmanager.h"
#include "Tutorial.h"
#include "LoadOut.h"
#include "NaviPointManager.h"
#include "ClearChecker.h"
#include "StageSelectManager.h"
#include "MenuManager.h"
#include "PlayerWeapon.h"
#include "ColliderManager.h"
#include "ParticleManager.h"
#include "FireBall.h"
#include "MagicMissile.h"
#include "Wind.h"

using namespace ImGui;
using namespace std;

void GameScene::Initialize()
{
	//ライト
	lightGroup_ = LightGroup::Create();
	//ライトの取得
	Object3d::SetLight(lightGroup_.get());
	FieldManager::SetLight(lightGroup_.get());
	FireBall::SetLight(lightGroup_.get());
	MagicMissile::SetLight(lightGroup_.get());
	Wind::SetLight(lightGroup_.get());
	PlayerWeapon::SetLight(lightGroup_.get());
	BaseEnemy::SetLight(lightGroup_.get());

	//カメラ
	Camera::GetInstance()->SetTarget(Vector3(0, 0, 0));
	Camera::GetInstance()->SetEye(Vector3(0, 1, -10));

	//判定
	ColliderManager::GetInstance()->Initialize();

	//敵
	EnemyManager::GetInstance()->Initialize();

	//クリアの判定
	ClearChecker::GetInstance()->Initialize();
	//グラウンド
	stageNum_ = StageSelectManager::GetInstance()->GetSelectNum();
	FieldManager::GetInstance()->Initialize(stageNum_);

	//プレイヤー
	Player::GetInstance()->Initialize();

	if (stageNum_ == TUTORIAL) {
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

	ParticleManager* particleM = ParticleManager::GetInstance();
	particleM->Initialize();

	gameOver_.Initialize();

	sound_.Initialize();
	sound_.LoadWave("mainBGM");
	sound_.RePlayWave("mainBGM", true);
	sound_.SetVolume("mainBGM", START_BOV);

	LoadOut::GetInstance()->Initialize();
	MenuManager::GetInstance()->Initialize();
}

void GameScene::GameInitialize()
{
	lightGroup_->PointLightAllClear();

	EnemyManager::GetInstance()->GameInitialize();
	FieldManager::GetInstance()->Initialize(stageNum_);
	Player::GetInstance()->GameInitialize();

	ClearChecker::GetInstance()->GameInitialize();
	gameOver_.Initialize();
	UIManager::GetInstance()->GameInitialize();
	sound_.RePlayWave("mainBGM", true);
	sound_.SetVolume("mainBGM", START_BOV);
	MenuManager::GetInstance()->GameInitialize();
	ColliderManager::GetInstance()->Initialize();
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
	ParticleManager* particleM = ParticleManager::GetInstance();
	LoadOut* loadOut = LoadOut::GetInstance();

#pragma endregion

#pragma region デバッグ用
#ifdef _DEBUG
	if (!particleM->GetIsEdit()) {

		if (input->TriggerKey(DIK_P)) {
			ParticleManager::GetInstance()->Clear();
			//GameManager::GetInstance()->SetTitleScene();
		}
		if (input->TriggerKey(DIK_R)) {
			Initialize();
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
			GameManager::GetInstance()->ChangeScene(make_unique<StageSelectScene>());
		}
		return;
	}

#pragma region Update呼び出し
	//Update呼び出し
	lightGroup_->Update();
	fieldM->Update();
	player->Update();//プレイヤー
	NaviPointManager::GetInstance()->CalcScore();
	enemyM->Update();//敵
	effectM->Update();
	spellM->Update();
	uiM->Update();
	ClearChecker::GetInstance()->Update();
	ColliderManager::GetInstance()->Update();
	particleM->Update();
	loadOut->Update();
	MenuManager::GetInstance()->Update();

#pragma endregion

#pragma region ImGui
#ifdef _DEBUG

		Begin("EnemyDebug");
		if (Button("stop", SIZE_IMGUI)) {
			Enemy::ToggleIsAllStop();
		}
		End();

		Begin("DebugButton");
		if (Button("Reset", SIZE_IMGUI)) {
			Initialize();
		}
		if (Button("EnemyPop", SIZE_IMGUI)) {
			enemyM->PopEnemy({ 0,0,0 });
		}
		if (Button("SlimePop", SIZE_IMGUI)) {
			enemyM->PopSlime({ 0,0,0 });
		}
		if (Button("ShowHitBox", SIZE_IMGUI)) {
			ColliderManager::GetInstance()->ToggleShowHitBox();
		}
		if (Button("particleClear", SIZE_IMGUI)) {
			ParticleManager::GetInstance()->Clear();
		}
		if (Button("PlayerInvincible", SIZE_IMGUI)) {
			Player::GetInstance()->SetInvincible();
		}
		End();

		Begin("Camera");
		Text("pos %f,%f,%f", Camera::GetInstance()->GetEye().x, Camera::GetInstance()->GetEye().y, Camera::GetInstance()->GetEye().z);
		End();

#endif // _DEBUG
#pragma endregion


	gameOver_.Update();

	//シーン遷移処理
	if (input->TriggerButton(XINPUT_GAMEPAD_START) || input->TriggerKey(DIK_ESCAPE)) {
		MenuManager::GetInstance()->SetGameMenu();
	}

	//Initialize系
	if (UIManager::GetInstance()->GetStateAlpha_() != 0 && (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_Z))) {
		GameInitialize();
		if (player->GetLife() > 0) {
			GameInitialize();
			GameManager::GetInstance()->ChangeScene(make_unique<StageSelectScene>());
			return;
		}
	}
	if (MenuManager::GetInstance()->GetIsReset()) {
		GameInitialize();
	}

	//シーン切り替え系
	if (MenuManager::GetInstance()->GetIsStageSelect()) {
		GameInitialize();
		GameManager::GetInstance()->ChangeScene(make_unique<StageSelectScene>());
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
	if (!ParticleManager::GetInstance()->GetIsEdit()) {
		FieldManager::GetInstance()->Draw();
		EffectManager::GetInstance()->Draw();
		SpellManager::GetInstance()->Draw();
		EnemyManager::GetInstance()->Draw();
		ColliderManager::GetInstance()->Draw();
	}
}

void GameScene::DrawTransparent()
{
	if (!ParticleManager::GetInstance()->GetIsEdit()) {
		EnemyManager::GetInstance()->DrawTransparent();
		Player::GetInstance()->Draw();
	}
	ClearChecker::GetInstance()->Draw();
}

void GameScene::ParticleDraw()
{
	ParticleManager::GetInstance()->Draw();
	//NaviPointManager::GetInstance()->Draw();
}

void GameScene::SpriteDraw()
{
	if (!ParticleManager::GetInstance()->GetIsEdit()) {

		if (!LoadOut::GetInstance()->GetIsActive()) {
			Player::GetInstance()->SpriteDraw();
			gameOver_.Draw();
		}
		UIManager::GetInstance()->Draw();
		ClearChecker::GetInstance()->SpriteDraw();
		MenuManager::GetInstance()->Draw();
	}
}

void GameScene::Finalize()
{
	ParticleManager::GetInstance()->Finalize();
}
