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
	triangleObj_->SetColor({0.8f,0.5f,0.2f,1});
	//ray
	rayModel_ = Model::LoadFromObj("box");
	rayObj_ = Object3d::Create();
	rayObj_->SetModel(rayModel_);
	//sphere2
	sphere2Model_ = Model::LoadFromObj("sphere");
	sphere2Obj_ = Object3d::Create();
	sphere2Obj_->SetModel(sphereModel_);

	planeWorldTransform_.scale = { 1,0.1f,1 };

	//球
	sphere_.center = XMVectorSet(0, 2, 0, 1);
	sphere_.radius = 0.9f;
	//平面
	plane_.normal = XMVectorSet(0, 1, 0, 0);
	plane_.distance = -0.1f;
	//三角形
	triangle_.p0 = XMVectorSet(-10.0f, -10.0f, -10, 1);
	triangle_.p1 = XMVectorSet(-10.0f, +10.0f, 10, 1);
	triangle_.p2 = XMVectorSet(10.0f, -10.0f, 10, 1);
	triangle_.normal = XMVectorSet(0.4082f, 0.8165f, -0.4082f, 0);
	//レイ
	ray_.start = XMVectorSet(0, 0, 0, 1);
	ray_.dir = XMVectorSet(0, -1, 0, 0);
	//球2
	sphere2_.center = XMVectorSet(-3, -5, 0, 1);
	sphere2_.radius = 0.9f;
}

void GameScene::Update(Input* input)
{
	//シーン切り替え
		if (input->PushKey(DIK_1)|| input->PushKey(DIK_2)|| input->PushKey(DIK_3)|| input->PushKey(DIK_4)|| input->PushKey(DIK_5)) {
			sphere_.center = XMVectorSet(0, 2, 0, 1);
			isMove_ = 0;
			timer_ = TIME_MAX;
			sphereObj_->SetColor(Vector4(1, 1, 1, 1));
			triangleObj_->SetColor(Vector4(0.8f,0.5f,0.2f,1 ));
			rayObj_->SetColor(Vector4(1, 1, 1, 1));
			planeObj_->SetColor(Vector4(1, 1, 1, 1));
			sphere2Obj_->SetColor(Vector4(1, 1, 1, 1));
			
			if (input->PushKey(DIK_1)) {
				scene_ = 0;
			}
			if (input->PushKey(DIK_2)) {
				sphere_.center = XMVectorSet(-1, -4, 0, 1);
				scene_ = 1;
			}
			if (input->PushKey(DIK_3)) {
				scene_ = 2;
			}
			if (input->PushKey(DIK_4)) {
				sphere_.center = XMVectorSet(2, 2, 0, 1);
				scene_ = 3;
			}
			if (input->PushKey(DIK_5)) {
				scene_ = 4;
			}
			
		}
		

	bool hit;
	XMVECTOR inter;
	float distance;
	XMVECTOR moveX;
	XMVECTOR moveY;
	
	if (--timer_ <= 0) {
		if (isMove_ == 0) {
			isMove_ = 1;
		}
		else if (isMove_ == 1) {
			isMove_ = 0;
		}
		timer_ = TIME_MAX;
	}

	switch (scene_)
	{
	case 0:
		moveY = XMVectorSet(0, 0.04f, 0, 0);
		if (isMove_ == false) {
			sphere_.center -= moveY;
		}
		else if (isMove_ == true) {
			sphere_.center += moveY;
		}

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
		moveX = XMVectorSet(0.15f, 0, 0, 0);
		if (isMove_ == false) {
			sphere_.center -= moveX;
		}
		else if (isMove_ == true) {
			sphere_.center += moveX;
		}

		//球と三角形の当たり判定
		hit = Collision::CheckSphere2Triangle(sphere_, triangle_, &inter);
		if (hit) {
			sphereObj_->SetColor(Vector4(1, 0, 0, 1));
		}
		else {
			sphereObj_->SetColor(Vector4(1, 1, 1, 1));
		}

		break;
	case 2:
		moveY = XMVectorSet(0, 0.1f, 0, 0);
		if (isMove_ == false) {
			sphere_.center -= moveY;
		}
		else if (isMove_ == true) {
			sphere_.center += moveY;
		}
		//レイの判定
		hit = Collision::CheckRay2Plane(ray_, plane_, &distance, &inter);
		if (hit) {
			rayObj_->SetColor(Vector4(1, 0, 0, 1));
			planeObj_->SetColor(Vector4(1, 0, 0, 1));
			float rayLen = inter.m128_f32[1] - sphere_.center.m128_f32[1];
			rayWorldTransform_.trans.y = sphere_.center.m128_f32[1] + rayLen / 2;
			rayWorldTransform_.scale.y = rayLen * 5;
		}
		else {
			rayObj_->SetColor(Vector4(1, 1, 1, 1));
			planeObj_->SetColor(Vector4(1, 1, 1, 1));
			rayWorldTransform_.trans.y = ray_.start.m128_f32[1] - 30;
			rayWorldTransform_.scale.y = 300;
		}

		break;
	case 3:
		moveX = XMVectorSet(0.15f, 0, 0, 0);
		if (isMove_ == false) {
			sphere_.center -= moveX;
		}
		else if (isMove_ == true) {
			sphere_.center += moveX;
		}
		hit = Collision::ChackRay2Triangle(ray_, triangle_, &distance, &inter);
		if (hit) {
			rayObj_->SetColor(Vector4(0, 1, 1, 1));
			triangleObj_->SetColor(Vector4(1, 0, 0, 1));
			float rayLen = inter.m128_f32[1] - sphere_.center.m128_f32[1];
			rayWorldTransform_.trans.y = sphere_.center.m128_f32[1] + rayLen / 2;
			rayWorldTransform_.scale.y = rayLen * 5;
		}else{
			rayObj_->SetColor(Vector4(1, 1, 1, 1));
			triangleObj_->SetColor(Vector4(1, 1, 1, 1));
			rayWorldTransform_.trans.y = ray_.start.m128_f32[1] - 30;
			rayWorldTransform_.scale.y = 300;
		}
		break;
	case 4:
		moveX = XMVectorSet(0.08f, 0, 0, 0);
		if (isMove_ == false) {
			sphere_.center -= moveX;
		}
		else if (isMove_ == true) {
			sphere_.center += moveX;
		}
		hit = Collision::CheckRay2Sphere(ray_,sphere2_,&distance,&inter);
		if (hit) {
			rayObj_->SetColor(Vector4(1, 0, 0, 1));
			sphere2Obj_->SetColor(Vector4(1, 0, 0, 1));
			float rayLen = inter.m128_f32[1] - sphere_.center.m128_f32[1];
			rayWorldTransform_.trans.y = sphere_.center.m128_f32[1] + rayLen / 2;
			rayWorldTransform_.scale.y = rayLen * 5;
		}
		else {
			rayObj_->SetColor(Vector4(1, 1, 1, 1));
			sphere2Obj_->SetColor(Vector4(1, 1, 1, 1));
			rayWorldTransform_.trans.y = ray_.start.m128_f32[1] - 30;
			rayWorldTransform_.scale.y = 300;
		}
		break;
	}
	ray_.start = sphere_.center;

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
	//rayWorldTransform_.trans.y = ray_.start.m128_f32[1] - 30;
	rayWorldTransform_.trans.z = ray_.start.m128_f32[2];
	//rayWorldTransform_.scale.y = 300;

	rayObj_->SetWorldTransform(rayWorldTransform_);
	rayObj_->Update();
	//sphere
	sphere2WorldTransform_.trans.x = sphere2_.center.m128_f32[0];
	sphere2WorldTransform_.trans.y = sphere2_.center.m128_f32[1];
	sphere2WorldTransform_.trans.z = sphere2_.center.m128_f32[2];

	sphere2Obj_->SetWorldTransform(sphere2WorldTransform_);
	sphere2Obj_->Update();

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
	case 3:
		rayObj_->Draw();
	case 1:
		triangleObj_->Draw();
		break;
	case 4:
		rayObj_->Draw();
		sphere2Obj_->Draw();
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