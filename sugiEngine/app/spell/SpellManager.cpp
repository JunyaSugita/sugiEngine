#include "SpellManager.h"
#include "Input.h"
#include "Player.h"

using namespace std;

SpellManager* SpellManager::GetInstance()
{
	static SpellManager instance;

	return &instance;
}

void SpellManager::Initialize()
{
	FireBall::OneTimeInitialize();
	MagicMissile::OneTimeInitialize();
	IceBolt::OneTimeInitialize();
	ChainLightning::OneTimeInitialize();
	for (unique_ptr<FireBall>& fireBall : fireBalls_) {
		fireBall->SetIsDead();
	}
	for (unique_ptr<MagicMissile>& magicMissile : magicMissiles_) {
		magicMissile->SetIsDead();
	}
	for (unique_ptr<IceBolt>& iceBolt : iceBolts_) {
		iceBolt->SetIsDead();
	}
	for (unique_ptr<ChainLightning>& chainLightning : chainLightnings_) {
		chainLightning->SetIsDead();
	}
	enchantFire_ = make_unique<EnchantFire>();
	enchantFire_->Initialize();

	maxCharge_ = 0;
	chargeTime_ = 0;
	useTime_ = 0;
	isUseFireBall_ = false;
	isUseMagicMissile_ = false;
	isUseIceBolt_ = false;
	isUseChainLightning_ = false;
}

void SpellManager::Update()
{
#pragma region インスタンス呼び出し
	//インスタンス呼び出し
	Input* input = Input::GetInstance();
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

	//各魔法のUpdate
	for (unique_ptr<FireBall>& fireBall : fireBalls_) {
		fireBall->Update();
	}
	for (unique_ptr<MagicMissile>& magicMissile : magicMissiles_) {
		magicMissile->Update();
	}
	for (unique_ptr<IceBolt>& iceBolt : iceBolts_) {
		iceBolt->Update();
	}
	for (unique_ptr<ChainLightning>& chainLightning : chainLightnings_) {
		chainLightning->Update();
	}
	enchantFire_->Update();

#pragma endregion

#pragma region 魔法の削除
	//消すフラグの立った魔法の削除
	fireBalls_.remove_if([](unique_ptr<FireBall>& fireBall) {
		return fireBall->GetIsDead();
		});
	magicMissiles_.remove_if([](unique_ptr<MagicMissile>& magicMissile) {
		return magicMissile->GetIsDead();
		});
	iceBolts_.remove_if([](unique_ptr<IceBolt>& iceBolt) {
		return iceBolt->GetIsDead();
		});
	chainLightnings_.remove_if([](unique_ptr<ChainLightning>& chainLightning) {
		return chainLightning->GetIsDead();
		});
#pragma endregion
}

void SpellManager::Draw()
{
	for (unique_ptr<FireBall>& fireBall : fireBalls_) {
		fireBall->Draw();
	}
	for (unique_ptr<MagicMissile>& magicMissile : magicMissiles_) {
		magicMissile->Draw();
	}
	for (unique_ptr<IceBolt>& iceBolt : iceBolts_) {
		iceBolt->Draw();
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

	Camera* camera = Camera::GetInstance();

	if (Input::GetInstance()->ReleaseKey(DIK_E) || Input::GetInstance()->ReleaseButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		if (int(chargeTime_ / TIME_CHARGE_FIREBALL)) {
			isUseFireBall_ = true;
			useTime_ = TIME_FIRE_FIREBALL;
		}
		chargeTime_ = 0;
	}
}

void SpellManager::FireFireBall()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = TIME_FIRE_FIREBALL;

	if (int(useTime_) == 1) {
		unique_ptr<FireBall> newSpell = make_unique<FireBall>();
		newSpell->Initialize(camera->GetEye(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		fireBalls_.push_back(move(newSpell));
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

	if (Input::GetInstance()->ReleaseKey(DIK_E) || Input::GetInstance()->ReleaseButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		if (int(chargeTime_ / TIME_CHARGE_MAGICMISSILE)) {
			isUseMagicMissile_ = true;
			useTime_ = TIME_FIRE_MAGICMISSILE;
		}
		chargeTime_ = 0;
	}
	Player::GetInstance()->SetIsSpell(false);
}

void SpellManager::FireMagicMissile()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = TIME_FIRE_MAGICMISSILE;

	if (int(useTime_) % 20 == 1) {
		unique_ptr<MagicMissile> newSpell = make_unique<MagicMissile>();
		newSpell->Initialize(camera->GetEye(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		magicMissiles_.push_back(move(newSpell));
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

	if (Input::GetInstance()->ReleaseKey(DIK_E) || Input::GetInstance()->ReleaseButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		if (int(chargeTime_ / TIME_CHARGE_ICEBOLT)) {
			isUseIceBolt_ = true;
			useTime_ = TIME_FIRE_ICEBOLT;
		}
		chargeTime_ = 0;
	}
	Player::GetInstance()->SetIsSpell(false);
}

void SpellManager::FireIceBolt()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = TIME_FIRE_ICEBOLT;

	if (int(useTime_) == 1) {
		unique_ptr<IceBolt> newSpell = make_unique<IceBolt>();
		newSpell->Initialize(camera->GetEye(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		iceBolts_.push_back(move(newSpell));
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

	if (Input::GetInstance()->ReleaseKey(DIK_E) || Input::GetInstance()->ReleaseButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		if (int(chargeTime_ / TIME_CHARGE_CHAINLIGHTNING)) {
			isUseChainLightning_ = true;
			useTime_ = TIME_FIRE_CHAINLIGHTNING;
		}
		chargeTime_ = 0;
	}
	Player::GetInstance()->SetIsSpell(false);
}

void SpellManager::FireChainLightning()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = TIME_FIRE_CHAINLIGHTNING;

	if (int(useTime_) == 1) {
		unique_ptr<ChainLightning> newSpell = make_unique<ChainLightning>();
		newSpell->Initialize(camera->GetEye(), camera->GetTarget() - camera->GetEye());
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

	if (Input::GetInstance()->ReleaseKey(DIK_E) || Input::GetInstance()->ReleaseButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		if (int(chargeTime_ / TIME_CHARGE_ENCHANTFIRE)) {
			isUseEnchantFire_ = true;
			useTime_ = TIME_FIRE_ENCHANTFIRE;
		}
		chargeTime_ = 0;
	}
	Player::GetInstance()->SetIsSpell(false);
}

void SpellManager::FireEnchantFire()
{
	maxCharge_ = TIME_FIRE_ENCHANTFIRE;

	if (int(useTime_) == 1) {
		enchantFire_->Fire();
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseEnchantFire_ = false;
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
	if (isUseFireBall_ || isUseMagicMissile_ || isUseIceBolt_ || isUseChainLightning_ || isUseEnchantFire_) {
		return true;
	}
	return false;
}
