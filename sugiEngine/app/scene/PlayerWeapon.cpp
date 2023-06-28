#include "PlayerWeapon.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Camera.h"

using namespace ImGui;

PlayerWeapon::PlayerWeapon()
{
}

PlayerWeapon::~PlayerWeapon()
{
}

PlayerWeapon* PlayerWeapon::GetInstance()
{
	static PlayerWeapon instance;

	return &instance;
}

void PlayerWeapon::Initialize()
{
	model_ = move(Model::LoadFromObj("box"));
	obj_ = move(Object3d::Create());
	obj_->SetModel(model_.get());
	
	pos_ = { 0,4,-50 };
	rot_ = { 30,0,0 };
	scale_ = { 1,10,1 };

	WorldTransUpdate();
}

void PlayerWeapon::Update(bool isAttack)
{
	//UŒ‚’†‚Í•Ší‚ðU‚é
	if (isAttack) {
		AttackMove();
	}
	//UŒ‚‚µ‚Ä‚¢‚È‚¢‚Æ‚«‚Í’ÊíŽ‚¿
	else {
		NormalMove();
	}

	WorldTransUpdate();
}

void PlayerWeapon::Draw()
{
	obj_->Draw();
}

void PlayerWeapon::NormalMove()
{

}

void PlayerWeapon::AttackMove()
{
	Camera* camera = Camera::GetInstance();

	pos_ = camera->GetEye();
	pos_.y = 6;
	pos_.z += 3;
	rot_ = { 90,-5,0 };

	obj_->SetColor({1,0,0,1});
}

void PlayerWeapon::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	SetWorldTrans();
}

void PlayerWeapon::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();
}

