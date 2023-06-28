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

void PlayerWeapon::Update(bool isAttack,bool isAttackOn)
{
	//UŒ‚’†‚Í•Ší‚ðU‚é
	if (isAttack) {
		AttackMove(isAttackOn);
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
	Camera* camera = Camera::GetInstance();
	Player* player = Player::GetInstance();

	float nowTime = player->GetTime();

	pos_ = camera->GetEye();
	pos_.x += 1.5f;
	pos_.y = 4;
	pos_.z += 3;
	rot_ = { 30,0,0 };

}

void PlayerWeapon::AttackMove(bool isAttackOn)
{
	Camera* camera = Camera::GetInstance();
	Player* player = Player::GetInstance();

	float nowTime = player->GetTime();

	pos_ = camera->GetEye();
	pos_.y = 4 + (nowTime / 30);
	pos_.z += 3;
	rot_ = { 120 - nowTime * 4,-5,0 };
	if (isAttackOn) {
		obj_->SetColor({ 1,0,0,1 });
	}
	else {
		obj_->SetColor({ 1,1,1,1 });
	}
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

