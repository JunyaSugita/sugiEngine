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

	worldTrans_.parent_ = &Player::GetInstance()->GetWorldTrans();
	pos_ = { 0,0,1 };
	rot_ = { 0,0,0 };
	scale_ = { 1,10,1 };

	WorldTransUpdate();
}

void PlayerWeapon::Update(bool isAttack)
{
	//攻撃中は武器を振る
	if (isAttack) {

	}
	//攻撃していないときは通常持ち
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

