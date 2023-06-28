#include "PlayerWeapon.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "EnemyManager.h"

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

	hitModel_ = move(Model::LoadFromObj("sphere",true));
	hitObj_ = move(Object3d::Create());
	hitObj_->SetModel(hitModel_.get());

	hitPos_ = { 0,0,0 };
	hitRot_ = { 0,0,0 };
	hitScale_ = { ATTACK_RADIUS,ATTACK_RADIUS,ATTACK_RADIUS };

	WorldTransUpdate();

	isAt_ = false;
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

	if (isAt_) {
		hitObj_->Draw();
	}
}

void PlayerWeapon::NormalMove()
{
	Player* player = Player::GetInstance();

	float nowTime = player->GetTime();

	pos_ = player->GetPos();
	pos_.x += float(sin(Radian(player->GetCameraAngle().x + 30)) * 4);
	pos_.y = 4;
	pos_.z += float(cos(Radian(player->GetCameraAngle().x + 30)) * 4);
	rot_ = { 30,player->GetCameraAngle().x,0 };

}

void PlayerWeapon::AttackMove(bool isAttackOn)
{
	Player* player = Player::GetInstance();

	float nowTime = player->GetTime();

	pos_ = player->GetPos();
	pos_.x += float(sin(Radian(player->GetCameraAngle().x)) * 2);
	pos_.y = 4 + (nowTime / 30);
	pos_.z += float(cos(Radian(player->GetCameraAngle().x)) * 2);
	rot_ = { 120 - nowTime * 4,player->GetCameraAngle().x - 5,0 };
	if (isAttackOn) {
		obj_->SetColor({ 1,0,0,1 });
		isAt_ = true;
		AttackCol();
	}
	else {
		obj_->SetColor({ 1,1,1,1 });
		isAt_ = false;
		EnemyManager::GetInstance()->ResetIsHit();
	}

	
}

void PlayerWeapon::AttackCol()
{
	//ƒCƒ“ƒXƒ^ƒ“ƒXŽæ“¾
	Player* player = Player::GetInstance();

	hitPos_ = player->GetPos();
	hitPos_.x += float(sin(Radian(player->GetCameraAngle().x)) * ATTACK_LENGTH);
	hitPos_.y = 0;
	hitPos_.z += float(cos(Radian(player->GetCameraAngle().x)) * ATTACK_LENGTH);

	WorldTransUpdate();
}

void PlayerWeapon::WorldTransUpdate()
{
	worldTrans_.SetPos(pos_);
	worldTrans_.SetRot(rot_);
	worldTrans_.SetScale(scale_);

	hitWorldTrans_.SetPos(hitPos_);
	hitWorldTrans_.SetRot(hitRot_);
	hitWorldTrans_.SetScale(hitScale_);

	SetWorldTrans();
}

void PlayerWeapon::SetWorldTrans()
{
	obj_->SetWorldTransform(worldTrans_);
	obj_->Update();

	hitObj_->SetWorldTransform(hitWorldTrans_);
	hitObj_->Update();
}

