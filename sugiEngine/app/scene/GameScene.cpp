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
	//���C�g
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());
	Fbx::SetLight(lightGroup_.get());

	//�J����
	Camera::GetInstance()->SetTarget(Vector3(0, 0, 0));
	Camera::GetInstance()->SetEye(Vector3(0, 1, -10));

	//�n��
	GroundManager::GetInstance()->Initialize();

	//�v���C���[
	Player::GetInstance()->Initialize();
	//�G
	EnemyManager::GetInstance()->Initialize();

	//�G�t�F�N�g
	EffectManager::GetInstance()->Initialize();
}

void GameScene::Update()
{
#pragma region �C���X�^���X�Ăяo��
	Input* input = Input::GetInstance();
	Player* player = Player::GetInstance();
	EnemyManager* enemyM = EnemyManager::GetInstance();
	GroundManager* groundM = GroundManager::GetInstance();
	EffectManager* effectM = EffectManager::GetInstance();

#pragma endregion

#pragma region ���C�g
	//���C�g
	lightGroup_->Update();
#pragma endregion

#pragma region �f�o�b�O�p

	if (input->TriggerKey(DIK_1)) {
		GameManager::GetInstance()->SetTitleScene();
	}

	if (input->TriggerKey(DIK_E)) {
		enemyM->PopEnemy();
	}
#pragma endregion

#pragma region Update�Ăяo��
	//Update�Ăяo��
	groundM->Update();
	player->Update();//�v���C���[
	enemyM->Update();//�G
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
