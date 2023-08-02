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

using namespace ImGui;
using namespace std;

void GameScene::Initialize()
{
	//���C�g
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());
	Fbx::SetLight(lightGroup_.get());

	//�J����
	Camera::GetInstance()->SetTarget(Vector3(0, 0, 0));
	Camera::GetInstance()->SetEye(Vector3(0, 1, -10));

	//�G
	EnemyManager::GetInstance()->Initialize();
	
	//�O���E���h
	FieldManager::GetInstance()->Initialize();

	//�v���C���[
	Player::GetInstance()->Initialize();


	//�G�t�F�N�g
	EffectManager::GetInstance()->Initialize();

	//���@
	SpellManager::GetInstance()->Initialize();

	//UI
	UIManager::GetInstance()->Initialize();

	//�����蔻��
	ColliderManager::GetInstance()->Initialize();

	ParticleManager* particleM = ParticleManager::GetInstance();
	particleM->Initialize();

	particleE_ = make_unique<ParticleEditor>();
	particleE_->Initialize();

	clearChecker_.Initialize();
	gameOver_.Initialize();

	sound_.Initialize();
	sound_.LoadWave("mainBGM");
	sound_.RePlayWave("mainBGM", true);
	sound_.SetVolume("mainBGM", 0.1f);

	LoadOut::GetInstance()->Initialize();
}

void GameScene::GameInitialize()
{
	EnemyManager::GetInstance()->GameInitialize();
	FieldManager::GetInstance()->GameInitialize();
	Player::GetInstance()->GameInitialize();

	clearChecker_.Initialize();
	gameOver_.Initialize();
	UIManager::GetInstance()->GameInitialize();
	sound_.RePlayWave("mainBGM", true);
}

void GameScene::Update()
{
#pragma region �C���X�^���X�Ăяo��
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

#pragma region �f�o�b�O�p
#ifdef _DEBUG
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
	}
#endif
#pragma endregion

#pragma region Update�Ăяo��
	//Update�Ăяo��
	lightGroup_->Update();
	fieldM->Update();
	player->Update();//�v���C���[
	enemyM->Update();//�G
	effectM->Update();
	spellM->Update();
	uiM->Update();
	colM->Update();
	particleM->Update();
	particleE_->Update();
	loadOut->Update();

#pragma endregion

#pragma region ImGui
#ifdef _DEBUG
	{
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

	//�V�[���J�ڏ���
	if (Tutorial::GetInstance()->GetIsTutorial() && !loadOut->GetIsActive() && (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_Z))) {
		Tutorial::GetInstance()->SetIsTutorial(false);
		GameInitialize();
	}
	else if (Tutorial::GetInstance()->GetIsTutorial()&&input->TriggerButton(XINPUT_GAMEPAD_START)) {
		loadOut->ToggleIsActive();
	}

	if (UIManager::GetInstance()->GetStateAlpha_() != 0 && (input->TriggerButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_Z))) {
		if (player->GetLife() > 0) {
			GameManager::GetInstance()->SetTitleScene();
		}
		else {
			GameInitialize();
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
	NavePointManager::GetInstance()->Draw();
}

void GameScene::SpriteDraw()
{
	if (!particleE_->GetIsEdit(0)) {

		if (!LoadOut::GetInstance()->GetIsActive()) {
			Player::GetInstance()->SpDraw();
			clearChecker_.Draw();
			gameOver_.Draw();
		}
		UIManager::GetInstance()->Draw();
	}
}

void GameScene::Finalize()
{
}
