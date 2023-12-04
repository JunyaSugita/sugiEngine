#include "SpellManager.h"
#include "Input.h"
#include "Player.h"
#include "PlayerWeapon.h"
#include "LoadOut.h"
#include "ImGuiManager.h"

using namespace std;
using namespace ImGui;

SpellManager* SpellManager::GetInstance()
{
	static SpellManager instance;

	return &instance;
}

void SpellManager::Initialize()
{
	for (unique_ptr<BaseSpell>& fireBall : spells_) {
		fireBall->SetIsDead();
	}
	for (unique_ptr<ChainLightning>& chainLightning : chainLightnings_) {
		chainLightning->SetIsDead();
	}

	maxCharge_ = 0;
	chargeTime_ = 0;
	useTime_ = 0;
	isUseFireBall_ = false;
	isUseMagicMissile_ = false;
	isUseIceBolt_ = false;
	isUseChainLightning_ = false;
	isUseEnchantFire_ = false;
	isUseFlame_ = false;
}

void SpellManager::Update()
{

#pragma region 状態のリセット
	isModeEnchantFire_ = false;

#pragma endregion

#pragma region 魔法のアップデート
	//各魔法の発射
	if (isUseFireBall_) {
		FireFireBall();
	}
	if (isUseMagicMissile_) {
		FireMagicMissile();
	}
	if (isUseIceBolt_) {
		FireIceBolt();
	}
	if (isUseChainLightning_) {
		FireChainLightning();
	}
	if (isUseEnchantFire_) {
		FireEnchantFire();
	}
	if (isUseFlame_) {
		FireFlame();
	}

	//各魔法のUpdate
	for (unique_ptr<BaseSpell>& fireBall : spells_) {
		fireBall->Update();
	}
	for (unique_ptr<ChainLightning>& chainLightning : chainLightnings_) {
		chainLightning->Update();
	}

#pragma endregion

#pragma region 魔法の削除
	//消すフラグの立った魔法の削除
	spells_.remove_if([](unique_ptr<BaseSpell>& fireBall) {
		return fireBall->GetIsDead();
		});
	chainLightnings_.remove_if([](unique_ptr<ChainLightning>& chainLightning) {
		return chainLightning->GetIsDead();
		});
#pragma endregion

#pragma region ImGui
#ifdef _DEBUG

	Begin("SpellDebug");
	Text("ChargeNow = %f", chargeTime_);
	Text("ChargeMax = %f", maxCharge_);
	Text("UseTime   = %f", useTime_);
	End();

#endif
#pragma endregion
}

void SpellManager::Draw()
{
	for (unique_ptr<BaseSpell>& fireBall : spells_) {
		fireBall->Draw();
	}
	for (unique_ptr<ChainLightning>& chainLightning : chainLightnings_) {
		chainLightning->Draw();
	}
}

void SpellManager::ChargeFireBall()
{
	maxCharge_ = TIME_CHARGE_FIREBALL;

	if (chargeTime_ < maxCharge_) {
		chargeTime_++;
	}

	if ((!Input::GetInstance()->PushKey(DIK_E) && !Input::GetInstance()->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) && !Input::GetInstance()->GetLTrigger()) || (LoadOut::GetInstance()->GetIsActive() && chargeTime_ >= maxCharge_)) {
		if (int(chargeTime_ / maxCharge_)) {
			isUseFireBall_ = true;
			useTime_ = TIME_FIRE_FIREBALL;
		}
		if (!(SpellManager::GetInstance()->ChargePercent() > 0.90f && SpellManager::GetInstance()->ChargePercent() < 1) && (isUseFireBall_ || !LoadOut::GetInstance()->GetIsActive())) {
			chargeTime_ = 0;
		}
	}
}

void SpellManager::FireFireBall()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = TIME_FIRE_FIREBALL;

	if (int(useTime_) == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<FireBall>();
		newSpell->Initialize(PlayerWeapon::GetInstance()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseFireBall_ = false;
		Player::GetInstance()->SetIsSpell(false);
	}

}

void SpellManager::ChargeMagicMissile()
{
	maxCharge_ = TIME_CHARGE_MAGICMISSILE;

	if (chargeTime_ < maxCharge_) {
		chargeTime_++;
	}

	if ((!Input::GetInstance()->PushKey(DIK_E) && !Input::GetInstance()->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) && !Input::GetInstance()->GetLTrigger()) || (LoadOut::GetInstance()->GetIsActive() && chargeTime_ >= maxCharge_)) {
		if (int(chargeTime_ / maxCharge_)) {
			isUseMagicMissile_ = true;
			useTime_ = TIME_FIRE_MAGICMISSILE;
		}
		if (!(SpellManager::GetInstance()->ChargePercent() > 0.90f && SpellManager::GetInstance()->ChargePercent() < 1) && (isUseMagicMissile_ || !LoadOut::GetInstance()->GetIsActive())) {
			chargeTime_ = 0;
		}
	}
	Player::GetInstance()->SetIsSpell(false);
}

void SpellManager::FireMagicMissile()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = TIME_FIRE_MAGICMISSILE;

	if (int(useTime_) % 20 == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<MagicMissile>();
		newSpell->Initialize(PlayerWeapon::GetInstance()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseMagicMissile_ = false;
		Player::GetInstance()->SetIsSpell(false);
	}

}

void SpellManager::ChargeIceBolt()
{
	maxCharge_ = TIME_CHARGE_ICEBOLT;

	if (chargeTime_ < maxCharge_) {
		chargeTime_++;
	}

	if ((!Input::GetInstance()->PushKey(DIK_E) && !Input::GetInstance()->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) && !Input::GetInstance()->GetLTrigger()) || (LoadOut::GetInstance()->GetIsActive() && chargeTime_ >= maxCharge_)) {
		if (int(chargeTime_ / maxCharge_)) {
			isUseIceBolt_ = true;
			useTime_ = TIME_FIRE_ICEBOLT;
		}
		if (!(SpellManager::GetInstance()->ChargePercent() > 0.90f && SpellManager::GetInstance()->ChargePercent() < 1) && (isUseIceBolt_ || !LoadOut::GetInstance()->GetIsActive())) {
			chargeTime_ = 0;
		}
	}
	Player::GetInstance()->SetIsSpell(false);
}

void SpellManager::FireIceBolt()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = TIME_FIRE_ICEBOLT;

	if (int(useTime_) == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<IceBolt>();
		newSpell->Initialize(PlayerWeapon::GetInstance()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseIceBolt_ = false;
		Player::GetInstance()->SetIsSpell(false);
	}
}

void SpellManager::ChargeChainLightning()
{
	maxCharge_ = TIME_CHARGE_CHAINLIGHTNING;

	if (chargeTime_ < maxCharge_) {
		chargeTime_++;
	}

	if ((!Input::GetInstance()->PushKey(DIK_E) && !Input::GetInstance()->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) && !Input::GetInstance()->GetLTrigger()) || (LoadOut::GetInstance()->GetIsActive() && chargeTime_ >= maxCharge_)) {
		if (int(chargeTime_ / maxCharge_)) {
			isUseChainLightning_ = true;
			useTime_ = TIME_FIRE_CHAINLIGHTNING;
		}
		if (!(SpellManager::GetInstance()->ChargePercent() > 0.90f && SpellManager::GetInstance()->ChargePercent() < 1) && (isUseChainLightning_ || !LoadOut::GetInstance()->GetIsActive())) {
			chargeTime_ = 0;
		}
	}
	Player::GetInstance()->SetIsSpell(false);
}

void SpellManager::FireChainLightning()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = maxCharge_;

	if (int(useTime_) == 1) {
		unique_ptr<ChainLightning> newSpell = make_unique<ChainLightning>();
		newSpell->Initialize(PlayerWeapon::GetInstance()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		chainLightnings_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseChainLightning_ = false;
		Player::GetInstance()->SetIsSpell(false);
	}

}

void SpellManager::ChargeEnchantFire()
{
	maxCharge_ = TIME_CHARGE_ENCHANTFIRE;

	if (chargeTime_ < maxCharge_) {
		chargeTime_++;
	}

	if ((!Input::GetInstance()->PushKey(DIK_E) && !Input::GetInstance()->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) && !Input::GetInstance()->GetLTrigger()) || (LoadOut::GetInstance()->GetIsActive() && chargeTime_ >= maxCharge_)) {
		if (int(chargeTime_ / maxCharge_)) {
			isUseEnchantFire_ = true;
			useTime_ = TIME_FIRE_ENCHANTFIRE;
		}
		if (!(SpellManager::GetInstance()->ChargePercent() > 0.90f && SpellManager::GetInstance()->ChargePercent() < 1) && (isUseEnchantFire_ || !LoadOut::GetInstance()->GetIsActive())) {
			chargeTime_ = 0;
		}
	}
	Player::GetInstance()->SetIsSpell(false);
}

void SpellManager::FireEnchantFire()
{
	maxCharge_ = TIME_FIRE_ENCHANTFIRE;

	if (int(useTime_) == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<EnchantFire>();
		newSpell->Initialize(PlayerWeapon::GetInstance()->GetOrbPos());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseEnchantFire_ = false;
		Player::GetInstance()->SetIsSpell(false);
	}

}

void SpellManager::ChargeFlame()
{
	maxCharge_ = TIME_CHARGE_FLAME;

	if (chargeTime_ < maxCharge_) {
		chargeTime_++;
	}

	if ((!Input::GetInstance()->PushKey(DIK_E) && !Input::GetInstance()->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) && !Input::GetInstance()->GetLTrigger()) || (LoadOut::GetInstance()->GetIsActive() && chargeTime_ >= maxCharge_)) {
		if (int(chargeTime_ / maxCharge_)) {
			isUseFlame_ = true;
			useTime_ = TIME_FIRE_FLAME;
		}
		if (!(SpellManager::GetInstance()->ChargePercent() > 0.90f && SpellManager::GetInstance()->ChargePercent() < 1) && (isUseFlame_ || !LoadOut::GetInstance()->GetIsActive())) {
			chargeTime_ = 0;
		}
	}
	Player::GetInstance()->SetIsSpell(false);
}

void SpellManager::FireFlame()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = TIME_FIRE_FLAME;

	if (int(useTime_) % 5 == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<Flame>();
		newSpell->Initialize(PlayerWeapon::GetInstance()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseFlame_ = false;
		Player::GetInstance()->SetIsSpell(false);
	}
}

float SpellManager::ChargePercent()
{
	if (maxCharge_ == 0.0f) {
		return 0.0f;
	}
	float temp = chargeTime_ / maxCharge_;
	if (temp > 1.0f) {
		return 1.0f;
	}
	return temp;
};

bool SpellManager::GetIsUseSpell()
{
	if (isUseFireBall_ || isUseMagicMissile_ || isUseIceBolt_ || isUseChainLightning_ || isUseEnchantFire_ || isUseFlame_) {
		return true;
	}
	return false;
}

int32_t SpellManager::GetSpellType(int32_t spell)
{
	switch (spell)
	{
	case FIRE_BALL:
	case ENCHANT_FIRE:
	case FLAME:

		return TYPE_FIRE;

	case MAGIC_MISSILE:
	case CHAIN_LIGHTNING:

		return TYPE_THUNDER;

	case ICE_BOLT:

		return TYPE_ICE;

	default:

		return TYPE_NONE;
	}
}
