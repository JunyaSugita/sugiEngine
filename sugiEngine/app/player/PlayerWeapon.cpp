#include "PlayerWeapon.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "EnemyManager.h"
#include "SpellManager.h"

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
	scale_ = { 0.1f,1,0.1f };

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
	else if (SpellManager::GetInstance()->GetIsUseSpell()) {
		SpellMove();
	}
	else if (SpellManager::GetInstance()->ChargeParcent() != 0.0f) {
		ChargeMove();
	}
	//UŒ‚‚µ‚Ä‚¢‚È‚¢‚Æ‚«‚Í’ÊíŽ‚¿
	else {
		NormalMove();
		a = 20;
	}

	WorldTransUpdate();
}

void PlayerWeapon::Draw()
{
	obj_->Draw();

	if (isAt_) {
		//hitObj_->Draw();
	}
}

void PlayerWeapon::NormalMove()
{
	Player* player = Player::GetInstance();

	pos_ = player->GetPos();
	pos_.x += float(sin(Radian(player->GetCameraAngle().x + 30)) * 4);
	pos_.y = 4;
	pos_.z += float(cos(Radian(player->GetCameraAngle().x + 30)) * 4);
	rot_ = { 30,player->GetCameraAngle().x,0 };

}

void PlayerWeapon::SpellMove()
{
	Player* player = Player::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();

	if (--a > 0) {
		Player* player = Player::GetInstance();

		float nowTime = a;

		pos_ = player->GetPos();
		pos_.x += float(sin(Radian(player->GetCameraAngle().x + 10)) * 2);
		pos_.y += float(sin(Radian(player->GetCameraAngle().y)) * 2 + 4.5f + (nowTime / 10));
		pos_.z += float(cos(Radian(player->GetCameraAngle().x + 10)) * 2);
		rot_ = { (player->GetCameraAngle().y + 90) * -1,player->GetCameraAngle().x,0 };
	}
	else {

		pos_ = player->GetPos();
		pos_.x += float(sin(Radian(player->GetCameraAngle().x + 10)) * 2);
		pos_.y += float(sin(Radian(player->GetCameraAngle().y)) * 2 + 4.5f);
		pos_.z += float(cos(Radian(player->GetCameraAngle().x + 10)) * 2);
		rot_ = { (player->GetCameraAngle().y + 90) * -1,player->GetCameraAngle().x,0 };
	}
}

void PlayerWeapon::ChargeMove()
{
	Player* player = Player::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();

	pos_ = player->GetPos();
	pos_.x += float(sin(Radian(player->GetCameraAngle().x + 30)) * 4);
	pos_.y = 4;
	pos_.z += float(cos(Radian(player->GetCameraAngle().x + 30)) * 4);
	rot_ = { 30 + float(sin(Radian(spellM->ChargeParcent() * 1000))* 15),player->GetCameraAngle().x,0 + float(cos(Radian(spellM->ChargeParcent() * 1000)) * 15) };
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

