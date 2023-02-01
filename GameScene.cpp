#include "GameScene.h"
#include "Collision.h"
#include <sstream>
#include <iomanip>

void GameScene::Initialize(int num)
{
	//sphere
	sphereModel_ = Model::LoadFromObj("sphere");
	sphereObj_ = Object3d::Create();
	sphereObj_->SetModel(sphereModel_);
	//plane
	planeModel_ = Model::LoadFromObj("ground");
	planeObj_ = Object3d::Create();
	planeObj_->SetModel(planeModel_);
	//triangle
	triangleModel_ = Model::LoadFromObj("triangle");
	triangleObj_ = Object3d::Create();
	triangleObj_->SetModel(triangleModel_);

	//sphereWorldTransform_.scale = { 1,1,1 };
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	planeWorldTransform_.scale = { 1,0.01f,1 };

	//球
	sphere_.center = XMVectorSet(0, 2, 0, 1);
	sphere_.radius = 0.9f;
	//平面
	plane_.normal = XMVectorSet(0, 1, 0, 0);
	plane_.distance = -0.1f;
	//三角形
	triangle_.p0 = XMVectorSet(-1.0f, -1.0f, 0, 1);
	triangle_.p1 = XMVectorSet(-1.0f, +1.0f, 0, 1);
	triangle_.p2 = XMVectorSet(1.0f, -1.0f, 0, 1);
	triangle_.normal = XMVectorSet(0.0f, 0, -1.0f, 0);

}

void GameScene::Update(Input* input)
{
	//球移動
	{
		XMVECTOR moveY = XMVectorSet(0, 0.1f, 0, 0);
		if (input->PushKey(DIK_W)) {
			sphere_.center += moveY;
		}
		else if (input->PushKey(DIK_S)) {
			sphere_.center -= moveY;
		}

		XMVECTOR moveX = XMVectorSet(0.1f, 0, 0, 0);
		if (input->PushKey(DIK_D)) {
			sphere_.center += moveX;
		}
		else if (input->PushKey(DIK_A)) {
			sphere_.center -= moveX;
		}
	}
	//シーン切り替え
	if (input->PushKey(DIK_1)) {
		scene_ = 0;
	}
	if (input->PushKey(DIK_2)) {
		scene_ = 1;
	}


	bool hit;
	switch (scene_)
	{
	case 0:
		//球と平面の当たり判定
		hit = Collision::CheckSphere2Plane(sphere_, plane_);
		if (hit) {
			sphereObj_->SetColor(Vector4(1, 0, 0, 1));
		}
		else {
			sphereObj_->SetColor(Vector4(1, 1, 1, 1));
		}
		break;
	case 1:
		//球と三角形の当たり判定
		XMVECTOR inter;
		hit = Collision::CheckSphere2Triangle(sphere_, triangle_, &inter);
		if (hit) {
			sphereObj_->SetColor(Vector4(1, 0, 0, 1));
		}
		else {
			sphereObj_->SetColor(Vector4(1, 1, 1, 1));
		}

		break;
	}

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
	switch (scene_)
	{
	case 0:
		planeObj_->Draw();
		break;
	case 1:
		triangleObj_->Draw();
		break;
	}
}

void GameScene::SpriteDraw()
{

}

void GameScene::Delete()
{
	delete sphereModel_;
	delete sphereObj_;
}
