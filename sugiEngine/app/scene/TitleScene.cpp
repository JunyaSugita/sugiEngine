#include "TitleScene.h"
#include "Input.h"

void TitleScene::Initialize()
{
	sphereModel_[0] = move(Model::LoadFromObj("sphere"));
	sphereModel_[1] = move(Model::LoadFromObj("sphere", true));

	for (uint32_t i = 0; i < 2; i++) {
		sphereObj_[i] = move(Object3d::Create());
		sphereObj_[i]->SetModel(sphereModel_[i].get());

		sphereWorldTransform_[i].SetScale(Vector3(10, 10, 10));

		sphereWorldTransform_[0].SetPos(Vector3(-10, -5, 0));
		sphereWorldTransform_[1].SetPos(Vector3(10, -5, 0));

		sphereObj_[i]->SetWorldTransform(sphereWorldTransform_[i]);
		sphereObj_[i]->Update();
	}

	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_.get());
}

void TitleScene::Update()
{
	for (uint32_t i = 0; i < 2; i++) {
		sphereWorldTransform_[i].AddRotZ(0.5f);

		sphereObj_[i]->SetWorldTransform(sphereWorldTransform_[i]);
		sphereObj_[i]->Update();
	}

	//ライト
	lightGroup_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		GameManager::GetInstance()->SetGameScene();
	}

	if (Input::GetInstance()->TriggerKey(DIK_3)) {
		GameManager::GetInstance()->SetClearScene();
	}
}

void TitleScene::BackSpriteDraw()
{

}

void TitleScene::Draw()
{
	sphereObj_[0]->Draw();
	sphereObj_[1]->Draw();
}

void TitleScene::ObjDraw()
{
}

void TitleScene::SpriteDraw()
{

}

void TitleScene::Delete()
{
}
