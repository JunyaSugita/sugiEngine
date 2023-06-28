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
	
	playerWorldTrans_ = Player::GetInstance()->GetWorldTrans();
	worldTrans_.parent_ = &playerWorldTrans_;
	pos_ = { 0,0,5 };
	rot_ = { 0,0,0 };
	scale_ = { 1,10,1 };

	WorldTransUpdate();
}

void PlayerWeapon::Update(bool isAttack)
{
	//ƒvƒŒƒCƒ„[‚ÌworldTransŽæ“¾
	playerWorldTrans_ = Player::GetInstance()->GetWorldTrans();

	//UŒ‚’†‚Í•Ší‚ðU‚é
	if (isAttack) {

	}
	//UŒ‚‚µ‚Ä‚¢‚È‚¢‚Æ‚«‚Í’ÊíŽ‚¿
	else {

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

