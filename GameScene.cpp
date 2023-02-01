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
	//ray
	rayModel_ = Model::LoadFromObj("box");
	rayObj_ = Object3d::Create();
	rayObj_->SetModel(rayModel_);

	//sphereWorldTransform_.scale = { 1,1,1 };
	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	planeWorldTransform_.scale = { 1,0.1f,1 };

	//��
	sphere_.center = XMVectorSet(0, 2, 0, 1);
	sphere_.radius = 0.9f;
	//����
	plane_.normal = XMVectorSet(0, 1, 0, 0);
	plane_.distance = -0.1f;
	//�O�p�`
	triangle_.p0 = XMVectorSet(-1.0f, -1.0f, 0, 1);
	triangle_.p1 = XMVectorSet(-1.0f, +1.0f, 0, 1);
	triangle_.p2 = XMVectorSet(1.0f, -1.0f, 0, 1);
	triangle_.normal = XMVectorSet(0.0f, 0, -1.0f, 0);
	//���C
	ray_.start = XMVectorSet(0, 0, 0, 1);
	ray_.dir = XMVectorSet(0, -1, 0, 0);

}

void GameScene::Update(Input* input)
{
	//���ړ�
	{
		XMVECTOR moveY = XMVectorSet(0, 0.1f, 0, 0);
		if (input->PushKey(DIK_W)) {
			sphere_.center += moveY;
			ray_.start = sphere_.center;
		}
		else if (input->PushKey(DIK_S)) {
			sphere_.center -= moveY;
			ray_.start = sphere_.center;
		}

		XMVECTOR moveX = XMVectorSet(0.1f, 0, 0, 0);
		if (input->PushKey(DIK_D)) {
			sphere_.center += moveX;
			ray_.start = sphere_.center;
		}
		else if (input->PushKey(DIK_A)) {
			sphere_.center -= moveX;
			ray_.start = sphere_.center;
		}
	}
	//�V�[���؂�ւ�
	if (input->PushKey(DIK_1)) {
		scene_ = 0;
	}
	if (input->PushKey(DIK_2)) {
		scene_ = 1;
	}
	if (input->PushKey(DIK_3)) {
		scene_ = 2;
		ray_.start = sphere_.center;
	}


	bool hit;
	XMVECTOR inter;
	switch (scene_)
	{
	case 0:
		//���ƕ��ʂ̓����蔻��
		hit = Collision::CheckSphere2Plane(sphere_, plane_);
		if (hit) {
			sphereObj_->SetColor(Vector4(1, 0, 0, 1));
		}
		else {
			sphereObj_->SetColor(Vector4(1, 1, 1, 1));
		}
		break;
	case 1:
		//���ƎO�p�`�̓����蔻��
		hit = Collision::CheckSphere2Triangle(sphere_, triangle_, &inter);
		if (hit) {
			sphereObj_->SetColor(Vector4(1, 0, 0, 1));
		}
		else {
			sphereObj_->SetColor(Vector4(1, 1, 1, 1));
		}

		break;
	case 2:
		//���C�̔���
		float distance;
		bool hit = Collision::CheckRay2Plane(ray_, plane_, &distance, &inter);
		if (hit) {
			rayObj_->SetColor(Vector4(1, 0, 0, 1));
			planeObj_->SetColor(Vector4(1, 0, 0, 1));
		}
		else {
			rayObj_->SetColor(Vector4(1, 1, 1, 1));
			planeObj_->SetColor(Vector4(1, 1, 1, 1));
		}

		break;
	}

	//sphere
	sphereWorldTransform_.trans.x = sphere_.center.m128_f32[0];
	sphereWorldTransform_.trans.y = sphere_.center.m128_f32[1];
	sphereWorldTransform_.trans.z = sphere_.center.m128_f32[2];

	sphereObj_->SetWorldTransform(sphereWorldTransform_);
	sphereObj_->Update();

	//plane
	planeWorldTransform_.trans.x = 0;
	planeWorldTransform_.trans.y = plane_.distance;
	planeWorldTransform_.trans.z = 0;

	planeObj_->SetWorldTransform(planeWorldTransform_);
	planeObj_->Update();

	//ray
	rayWorldTransform_.trans.x = ray_.start.m128_f32[0];
	rayWorldTransform_.trans.y = ray_.start.m128_f32[1] - 30;
	rayWorldTransform_.trans.z = ray_.start.m128_f32[2];
	rayWorldTransform_.scale.y = 300;

	rayObj_->SetWorldTransform(rayWorldTransform_);
	rayObj_->Update();
}

void GameScene::BackSpriteDraw()
{

}

void GameScene::Draw()
{
	sphereObj_->Draw();
	switch (scene_)
	{
	case 2:
		rayObj_->Draw();
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
	delete planeModel_;
	delete planeObj_;
	delete triangleModel_;
	delete triangleObj_;
	delete rayModel_;
	delete rayObj_;
}