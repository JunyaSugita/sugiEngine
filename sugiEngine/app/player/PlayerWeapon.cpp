#include "PlayerWeapon.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "EnemyManager.h"
#include "SpellManager.h"
#include "ParticleManager.h"
#include "ModelManager.h"
#include "ClearChecker.h"
#include "LoadOut.h"

LightGroup* PlayerWeapon::lightGroup_ = nullptr;

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
	orbObj_.obj->SetColor({ 0,1,1,0.5f });
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

	alpha_ = 1;
}

void PlayerWeapon::Update(bool isAttack, bool isAttackOn)
{
	if (ClearChecker::GetInstance()->GetIsClear()) {
		alpha_ -= SPEED_ALPHA;
		obj_.obj->SetColor(COLOR_WEAPON ,alpha_ );
		orbObj_.obj->SetColor( COLOR_ORB,alpha_ * ALPHA_ORB);
		WorldTransUpdate();
		return;
	}
	else {
		obj_.obj->SetColor(COLOR_WEAPON,1);
		orbObj_.obj->SetColor(COLOR_ORB, ALPHA_ORB);
	}

	//攻撃中は武器を振る
	if (isAttack) {
		AttackMove(isAttackOn);
		weaponY = START_WEAPON_Y;
	}
	else if (SpellManager::GetInstance()->GetIsUseSpell()) {
		SpellMove();
		weaponY = START_WEAPON_Y;
	}
	else if (SpellManager::GetInstance()->ChargePercent() != 0.0f) {
		ChargeMove();
		weaponY = START_WEAPON_Y;
	}
	//攻撃していないときは通常持ち
	else {
		NormalMove();
		easeTimer_ = EASING_TIME;
	}

	WorldTransUpdate();
	if (SpellManager::GetInstance()->GetActiveEnchantFire()) {
		if (useLightNum_ == -1) {
			useLightNum_ = lightGroup_->SetPointLightGetNum();
			lightGroup_->SetPointLightColor(useLightNum_, COLOR_FIRE_LIGHT);
			lightGroup_->SetPointLightAtten(useLightNum_, ATTEN_FIRE);
		}
		ParticleManager::GetInstance()->AddFromFile(P_WEAPON_FIRE, orbObj_.worldTrans.GetMatPos());
		lightGroup_->SetPointLightPos(useLightNum_, { orbObj_.worldTrans.GetMatPos().x, orbObj_.worldTrans.GetMatPos().y ,orbObj_.worldTrans.GetMatPos().z });
	}
	else if (useLightNum_ != -1) {
		lightGroup_->SetPointLightActive(useLightNum_, false);
		useLightNum_ = -1;
	}
}

void PlayerWeapon::Draw()
{
	obj_.Draw();
	orbObj_.Draw();
}

void PlayerWeapon::NormalMove()
{
	Player* player = Player::GetInstance();

	obj_.pos = player->GetPos();
	obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x + POS_WEAPON)) * LEN_WEAPON);
	obj_.pos.y = PATCH_WEAPON_Y - weaponY;
	obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x + POS_WEAPON)) * LEN_WEAPON);
	obj_.rot = { POS_WEAPON,player->GetCameraAngle().x,0 };

	//武器持ち上げ
	if (weaponY > 0) {
		weaponY -= SPEED_REVERSE_WEAPON;
	}
}

void PlayerWeapon::SpellMove()
{
	Player* player = Player::GetInstance();

	if (--easeTimer_ > 0) {
		float nowTime = easeTimer_ / PATCH_EASING_TIME;

		obj_.pos = player->GetPos();
		obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x + PATCH_SPELLMOVE_LEN)) * LEN_MOVE_WEAPON);
		obj_.pos.y += float(sin(Radian(player->GetCameraAngle().y)) * LEN_MOVE_WEAPON + PATCH_SPELLMOVE + EaseOut(nowTime, LEN_MOVE_WEAPON));
		obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x + PATCH_SPELLMOVE_LEN)) * LEN_MOVE_WEAPON);
		obj_.rot = { (player->GetCameraAngle().y - 90) * -1,player->GetCameraAngle().x,0 };
	}
	else {

		obj_.pos = player->GetPos();
		obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x + PATCH_SPELLMOVE_LEN)) * LEN_MOVE_WEAPON);
		obj_.pos.y += float(sin(Radian(player->GetCameraAngle().y)) * LEN_MOVE_WEAPON + PATCH_SPELLMOVE);
		obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x + PATCH_SPELLMOVE_LEN)) * LEN_MOVE_WEAPON);
		obj_.rot = { (player->GetCameraAngle().y - RAD / 2) * -1,player->GetCameraAngle().x,0 };
	}
}

void PlayerWeapon::ChargeMove()
{
	Player* player = Player::GetInstance();
	SpellManager* spellM = SpellManager::GetInstance();

	obj_.pos = player->GetPos();
	obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x + POS_WEAPON)) * LEN_WEAPON);
	obj_.pos.y = PATCH_WEAPON_Y;
	obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x + POS_WEAPON)) * LEN_WEAPON);
	obj_.rot = { POS_WEAPON + float(sin(Radian(spellM->ChargePercent() * 1000)) * PATCH_CHARGEMOVE),player->GetCameraAngle().x,0 + float(cos(Radian(spellM->ChargePercent() * 1000)) * PATCH_CHARGEMOVE) };

	if (!LoadOut::GetInstance()->GetIsActive()) {
		PopChargeParticle(spellM->GetSpellType(LoadOut::GetInstance()->GetSpell(Player::GetInstance()->GetPresetSpell())));
	}
}

void PlayerWeapon::AttackMove(bool isAttackOn)
{
	Player* player = Player::GetInstance();

	float nowTime = player->GetTime();

	float easeTime = nowTime / PATCH_ATTACK_EASING_TIME;
	if (easeTime >= 1) {
		easeTime = 1;
	}

	obj_.pos = player->GetPos();
	obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x)) * LEN_MOVE_WEAPON);
	obj_.pos.y = EaseOut(easeTime, PATCH_ATTACKMOVE_EASING);
	obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x)) * LEN_MOVE_WEAPON);
	obj_.rot = { PATCH_ATTACKMOVE - EaseIn(easeTime,PATCH_ATTACKMOVE),player->GetCameraAngle().x - PATCH_ATTACKMOVE_LEN,0 };
	if (isAttackOn) {
		isAt_ = true;
		AttackCol();
	}
	else {
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

void PlayerWeapon::PopChargeParticle(int32_t num)
{
	//呪文の属性ごとに色を変えたパーティクルを出す
	switch (num)
	{
	case TYPE_FIRE:
		ChargeParticle(COLOR_FIRE);
		break;
	case TYPE_THUNDER:
		ChargeParticle(COLOR_THUNDER);
		break;
	case TYPE_ICE:
		ChargeParticle(COLOR_ICE);
		break;
	case TYPE_DARK:
		ChargeParticle(COLOR_DARK);
		break;
	default:
		ChargeParticle(COLOR_DEF);
		break;
	}
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

void PlayerWeapon::ChargeParticle(Vector4 color)
{
	if (SpellManager::GetInstance()->ChargePercent() != 1.0f) {
		ParticleManager::GetInstance()->AddFromFileEditScaleAndColor(P_CHARGE_FIRE, orbObj_.worldTrans.GetMatPos(), SpellManager::GetInstance()->ChargePercent(), color);
		if (SpellManager::GetInstance()->ChargePercent() >= MAGIC_COYOTE) {
			ParticleManager::GetInstance()->AddFromFile(P_CHARGE_MAX_FIRE, orbObj_.worldTrans.GetMatPos());
		}
	}
	else {
		ParticleManager::GetInstance()->AddFromFileEditScaleAndColor(P_CHARGE_FIRE, orbObj_.worldTrans.GetMatPos(), 0.8f, color);
	}
}
