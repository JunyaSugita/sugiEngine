#include "TitleScene.h"
#include "Input.h"

void TitleScene::Initialize()
{
	sphereModel_[0] = Model::LoadFromObj("sphere");
	sphereModel_[1] = Model::LoadFromObj("sphere",true);

	for (int i = 0; i < 2; i++) {
		sphereObj_[i] = Object3d::Create();
		sphereObj_[i]->SetModel(sphereModel_[i]);

		sphereWorldTransform_[i].scale_ = { 10,10,10 };

		sphereWorldTransform_[0].trans_ = { -10,-5,0 };
		sphereWorldTransform_[1].trans_ = { 0,-5,0 };

		sphereObj_[i]->SetWorldTransform(sphereWorldTransform_[i]);
		sphereObj_[i]->Update();
	}


	//ライト
	lightGroup_ = LightGroup::Create();
	Object3d::SetLight(lightGroup_);

}

void TitleScene::Update()
{
	for (int i = 0; i < 2; i++) {
		sphereWorldTransform_[i].rotation.z += 0.5f;

		sphereObj_[i]->SetWorldTransform(sphereWorldTransform_[i]);
		sphereObj_[i]->Update();
	}

	//ライト
	lightGroup_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		//GameManager::GetInstance()->SetGameScene();
	}
}

void TitleScene::BackSpriteDraw()
{

}

void TitleScene::Draw()
{
	// sphereObj_[0]->Draw();
	sphereObj_[1]->Draw();
}

void TitleScene::SpriteDraw()
{

}

void TitleScene::Delete()
{
	for (int i = 0; i < 2; i++) {
		delete sphereModel_[i];
		delete sphereObj_[i];
	}

	delete lightGroup_;
}
