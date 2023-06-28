#include "PlayerWeapon.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "Input.h"

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
	
	pos_ = { 0,4,-49 };
	rot_ = { 0,0,0 };
	scale_ = { 1,10,1 };

	WorldTransUpdate();
}

void PlayerWeapon::Update(bool isAttack)
{
	Player* player = Player::GetInstance();
	//�U�����͕����U��
	if (isAttack) {
		pos_ = player->GetPos();
		pos_.y += 4.5f;
		pos_.z += 2.0f;
		rot_ = { 90,0,0 };
	}
	//�U�����Ă��Ȃ��Ƃ��͒ʏ펝��
	else {
		NormalMove();
		Begin("weapon");
		Text("length %f", (player->GetPos() - pos_).length());
		End();
		
	}

	WorldTransUpdate();
}

void PlayerWeapon::Draw()
{
	obj_->Draw();
}

void PlayerWeapon::NormalMove()
{
	//�C���X�^���X�擾
	Input* input = Input::GetInstance();
	Player* player = Player::GetInstance();

	Vector3 moveZ = { player->GetFrontVec().x,0, player->GetFrontVec().z};
	moveZ.normalize();
	Vector3 moveX = { player->GetRightVec().x,0,player->GetRightVec().z };
	moveX.normalize();
	//�ړ�
	if (input->PushKey(DIK_W)) {
		pos_ += moveZ * SPEED_MOVE;
	}
	if (input->PushKey(DIK_S)) {
		pos_ -= moveZ * SPEED_MOVE;
	}
	if (input->PushKey(DIK_A)) {
		pos_ -= moveX * SPEED_MOVE;
	}
	if (input->PushKey(DIK_D)) {
		pos_ += moveX * SPEED_MOVE;
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

