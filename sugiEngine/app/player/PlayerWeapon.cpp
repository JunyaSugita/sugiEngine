#include "PlayerWeapon.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "EnemyManager.h"
#include "SpellManager.h"
#include "ParticleManager.h"
#include "ModelManager.h"

using namespace ImGui;

PlayerWeapon* PlayerWeapon::GetInstance()
{
	static PlayerWeapon instance;

	return &instance;
}

void PlayerWeapon::Initialize()
{
	obj_.Initialize("weapon");
	obj_.pos = { 0,3.5f,-50 };
	obj_.rot = { 30,0,0 };
	obj_.scale = { 1,1,1 };

	orbObj_.Initialize("orb");
	orbObj_.obj->SetColor({0,1,1,0.5f});
	orbObj_.obj->SetIsSimple();

	orbObj_.pos = { 0,1.7f,0 };
	orbObj_.rot = { 0,0,0 };
	orbObj_.scale = { 0.3f,0.3f,0.3f };
	orbObj_.worldTrans.parent_ = &obj_.worldTrans;

	hitObj_ = move(Object3d::Create());
	hitObj_->SetModel(ModelManager::GetInstance()->Get("box"));

	hitPos_ = { 0,0,0 };
	hitRot_ = { 0,0,0 };
	hitScale_ = { ATTACK_RADIUS,ATTACK_RADIUS,ATTACK_RADIUS };

	WorldTransUpdate();

	isAt_ = false;

}

void PlayerWeapon::Update(bool isAttack,bool isAttackOn)
{
	//攻撃中は武器を振る
	if (isAttack) {
		AttackMove(isAttackOn);
		
	}
	else if (SpellManager::GetInstance()->GetIsUseSpell()) {
		SpellMove();
	}
	else if (SpellManager::GetInstance()->ChargePercent() != 0.0f) {
		ChargeMove();
	}
	//攻撃していないときは通常持ち
	else {
		NormalMove();
		easeTimer_ = 20;
	}

	WorldTransUpdate();
	if (SpellManager::GetInstance()->GetActiveEnchantFire()) {
		ParticleManager::GetInstance()->AddFromFile(P_WEAPON_FIRE, orbObj_.worldTrans.GetMatPos());
	}
}

void PlayerWeapon::Draw()
{
	obj_.Draw();
	orbObj_.Draw();

	if (isAt_) {
		//hitObj_->Draw();
	}
}

void PlayerWeapon::NormalMove()
{
	Player* player = Player::GetInstance();

	obj_.pos = player->GetPos();
	obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x + 30)) * 4);
	obj_.pos.y = 3.5f;
	obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x + 30)) * 4);
	obj_.rot = { 30,player->GetCameraAngle().x,0 };

}

void PlayerWeapon::SpellMove()
{
	Player* player = Player::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();

	if (--easeTimer_ > 0) {
		Player* player = Player::GetInstance();

		float nowTime = easeTimer_ / 19;

		obj_.pos = player->GetPos();
		obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x + 10)) * 2);
		obj_.pos.y += float(sin(Radian(player->GetCameraAngle().y)) * 2 + 4.5f + EaseOut(nowTime,2));
		obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x + 10)) * 2);
		obj_.rot = { (player->GetCameraAngle().y - 90) * -1,player->GetCameraAngle().x,0 };
	}
	else {

		obj_.pos = player->GetPos();
		obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x + 10)) * 2);
		obj_.pos.y += float(sin(Radian(player->GetCameraAngle().y)) * 2 + 4.5f);
		obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x + 10)) * 2);
		obj_.rot = { (player->GetCameraAngle().y - 90) * -1,player->GetCameraAngle().x,0 };
	}
}

void PlayerWeapon::ChargeMove()
{
	Player* player = Player::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();

	obj_.pos = player->GetPos();
	obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x + 30)) * 4);
	obj_.pos.y = 3.5f;
	obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x + 30)) * 4);
	obj_.rot = { 30 + float(sin(Radian(spellM->ChargePercent() * 1000))* 15),player->GetCameraAngle().x,0 + float(cos(Radian(spellM->ChargePercent() * 1000)) * 15) };
}

void PlayerWeapon::AttackMove(bool isAttackOn)
{
	Player* player = Player::GetInstance();

	float nowTime = player->GetTime();

	float easeTime = nowTime / 45.0f;
	if (easeTime >= 1.0f) {
		easeTime = 1.0f;
	}

	obj_.pos = player->GetPos();
	obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x)) * 2);
	obj_.pos.y = EaseOut(easeTime,6);
	obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x)) * 2);
	obj_.rot = { 130 - EaseIn(easeTime,130),player->GetCameraAngle().x - 5,0 };
	if (isAttackOn) {
		//obj_->SetColor({ 1,0,0,1 });
		isAt_ = true;
		AttackCol();
	}
	else {
		obj_.obj->SetColor({ 1,1,1,1 });
		isAt_ = false;
		EnemyManager::GetInstance()->ResetIsHit();
	}

	WorldTransUpdate();
	ParticleManager::GetInstance()->AddFromFile(P_WEAPON, orbObj_.worldTrans.GetMatPos());
}

void PlayerWeapon::AttackCol()
{
	//インスタンス取得
	Player* player = Player::GetInstance();

	hitPos_ = player->GetPos();
	hitPos_.x += float(sin(Radian(player->GetCameraAngle().x)) * ATTACK_LENGTH);
	hitPos_.y = 0;
	hitPos_.z += float(cos(Radian(player->GetCameraAngle().x)) * ATTACK_LENGTH);
}

void PlayerWeapon::WorldTransUpdate()
{
	hitWorldTrans_.SetPos(hitPos_);
	hitWorldTrans_.SetRot(hitRot_);
	hitWorldTrans_.SetScale(hitScale_);

	hitObj_->SetWorldTransform(hitWorldTrans_);
	hitObj_->Update();

	obj_.Update();
	orbObj_.Update();
}
