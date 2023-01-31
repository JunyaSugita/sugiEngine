#include "GameScene.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>

void GameScene::Initialize(int num)
{
	sphereModel_ = Model::LoadFromObj("sphere");
	sphereObj_ = Object3d::Create();
	sphereObj_->SetModel(sphereModel_);
	planeModel_ = Model::LoadFromObj("ground");
	planeObj_ = Object3d::Create();
	planeObj_->SetModel(planeModel_);

	//sphereWorldTransform_.scale = { 1,1,1 };
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	planeWorldTransform_.scale = { 1,0.01f,1 };

	//‹…
	sphere_.center = XMVectorSet(0, 2, 0, 1);
	sphere_.radius = 1.0f;
	//•½–Ê
	plane_.normal = XMVectorSet(0, 1, 0, 0);
	plane_.distance = -0.1f;

}

void GameScene::Update(Input* input)
{
	//‹…ˆÚ“®
	{
		XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
		if (input->PushKey(DIK_S)) {
			sphere_.center += moveY;
		}
		else if (input->PushKey(DIK_W)) {
			sphere_.center -= moveY;
		}

		XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_D)) {
			sphere_.center += moveX;
		}
		else if (input->PushKey(DIK_A)) {
			sphere_.center -= moveX;
		}
	}

	//‹…‚Æ•½–Ê‚Ì“–‚½‚è”»’è
	bool hit = Collision::CheckSphere2Plane(sphere_, plane_);
	if (hit) {
		sphereObj_->SetColor({1,0,0,1});
	}
	else {
		sphereObj_->SetColor({ 1,1,1,1 });
	}
	sphereObj_->Update();

	sphereWorldTransform_.trans.x = sphere_.center.m128_f32[0];
	sphereWorldTransform_.trans.y = sphere_.center.m128_f32[1];
	sphereWorldTransform_.trans.z = sphere_.center.m128_f32[2];

	planeWorldTransform_.trans.x = 0;
	planeWorldTransform_.trans.y = plane_.distance;
	planeWorldTransform_.trans.z = 0;

	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	planeObj_->SetWorldTransform(planeWorldTransform_);
	planeObj_->Update();
}

void GameScene::BackSpriteDraw()
{

}

void GameScene::Draw()
{
	sphereObj_->Draw();
	planeObj_->Draw();
}

void GameScene::SpriteDraw()
{

}

void GameScene::Delete()
{
	delete sphereModel_;
	delete sphereObj_;
}
