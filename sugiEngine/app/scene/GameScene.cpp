#include "GameScene.h"
#include "Input.h"
#include "FbxLoader.h"
#include "PostEffect.h"

#include "Player.h"
#include "EnemyManager.h"


using namespace ImGui;

void GameScene::Initialize()
{
	groundModel_ = move(Model::LoadFromObj("ground"));
	groundObj_ = move(Object3d::Create());
	groundObj_->SetModel(groundModel_.get());

	groundWorldTransform_.SetPos(Vector3(0, 0, 0));
	groundObj_->SetWorldTransform(groundWorldTransform_);
	groundObj_->Update();

	//���C�g
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());
	Fbx::SetLight(lightGroup_.get());

	//�J����
	Camera::GetInstance()->SetTarget(Vector3(0, 0, 0));
	Camera::GetInstance()->SetEye(Vector3(0, 1, -10));

	//�v���C���[
	Player::GetInstance()->Initialize();
	//�G
	EnemyManager::GetInstance()->Initialize();
}

void GameScene::Update()
{
#pragma region �C���X�^���X�Ăяo��
	Input* input = Input::GetInstance();
	Player* player = Player::GetInstance();
	EnemyManager* enemyM = EnemyManager::GetInstance();

#pragma endregion

	groundObj_->SetWorldTransform(groundWorldTransform_);
	groundObj_->Update();

	//���C�g
	lightGroup_->Update();

	if (input->TriggerKey(DIK_1)) {
		GameManager::GetInstance()->SetTitleScene();
	}

	if (input->TriggerKey(DIK_SPACE)) {
		enemyM->Set();
	}

#pragma region Update�Ăяo��
	//Update�Ăяo��
	player->Update();//�v���C���[
	enemyM->Update();//�G
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

		Begin("Object");
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
	groundObj_->Draw();
	EnemyManager::GetInstance()->Draw();
}

void GameScene::SpriteDraw()
{
}

void GameScene::Finalize()
{
}
