#include "StageSelectScene.h"
#include "SceneChange.h"
#include "StageSelectManager.h"
#include "Input.h"
#include "FieldManager.h"
#include "ParticleManager.h"
#include "EnemyManager.h"

void StageSelectScene::Initialize()
{
	StageSelectManager::GetInstance()->Initialize();
	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());
	FieldManager::SetLight(lightGroup_.get());
	BaseEnemy::SetLight(lightGroup_.get());
	//敵
	EnemyManager::GetInstance()->Initialize();
	FieldManager::GetInstance()->Initialize(0);
	ParticleManager::GetInstance()->Initialize();

	Camera::GetInstance()->SetEye({ 3,5,3 });
	Camera::GetInstance()->SetTarget({ 0,5,10 });

	//敵の動きを止める
	Enemy::SetIsAllStop(true);
}

void StageSelectScene::Update()
{
	//ステージ選択
	StageSelectManager::GetInstance()->Update();
	//シーンチェンジ
	SceneChange::GetInstance()->Update();

	//Update呼び出し
	lightGroup_->Update();
	FieldManager::GetInstance()->Update();
	ParticleManager::GetInstance()->Update();
	EnemyManager::GetInstance()->Update();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_A)) {
		SceneChange::GetInstance()->Start();
	}
	if (SceneChange::GetInstance()->GetTimer() >= 1.0f) {
		GameManager::GetInstance()->ChangeScene(std::make_unique<GameScene>());
	}
}

void StageSelectScene::BackSpriteDraw()
{
}

void StageSelectScene::Draw()
{
}

void StageSelectScene::ObjDraw()
{
	FieldManager::GetInstance()->Draw();
	EnemyManager::GetInstance()->Draw();
}

void StageSelectScene::DrawTransparent()
{

}

void StageSelectScene::ParticleDraw()
{
	ParticleManager::GetInstance()->Draw();
}

void StageSelectScene::SpriteDraw()
{
	StageSelectManager::GetInstance()->Draw();
	SceneChange::GetInstance()->Draw();
}

void StageSelectScene::Finalize()
{

}
