#include "PlayerWeapon.h"
#include "Player.h"

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
	
	pos_ = { 0,0,0 };
	rot_ = { 0,0,0 };
	scale_ = { 1,10,1 };

	WorldTransUpdate();
}

void PlayerWeapon::Update(bool isAttack)
{
	Player* player = Player::GetInstance();
	//UŒ‚’†‚Í•Ší‚ðU‚é
	if (isAttack) {
		pos_ = player->GetPos();
		pos_.y += 4.5f;
		pos_.z += 2.0f;
		rot_ = { 90,0,0 };
	}
	//UŒ‚‚µ‚Ä‚¢‚È‚¢‚Æ‚«‚Í’ÊíŽ‚¿
	else {
		pos_ = player->GetPos();
		pos_.x += 1.0f + float(sin(Radian(player->GetCameraAngle().x)) * 1);
		pos_.y += 4.0f;
		pos_.z += 1.0f + float(cos(Radian(player->GetCameraAngle().x)) * 1);
		rot_.x = 30;
		rot_.y = player->GetCameraAngle().x;
	}

	WorldTransUpdate();
}

void PlayerWeapon::Draw()
{
	obj_->Draw();
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

