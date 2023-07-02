#include "SpellManager.h"
#include "Input.h"
#include "Player.h"

SpellManager::SpellManager()
{
}

SpellManager::~SpellManager()
{
}

SpellManager* SpellManager::GetInstance()
{
	static SpellManager instance;

	return &instance;
}

void SpellManager::Initialize()
{
	FireBall::OneTimeInitialize();
	MagicMissile::OneTimeInitialize();
	for (unique_ptr<FireBall>& fireBall : fireBalls_) {
		fireBall->SetIsDead();
	}
	for (unique_ptr<MagicMissile>& magicMissile : magicMissiles_) {
		magicMissile->SetIsDead();
	}

	maxCharge_ = 0;
	chargeTime_ = 0;
	useTime_ = 0;
	isUseFireBall_ = false;
	isUseMagicMissile_ = false;
}

void SpellManager::Update()
{
#pragma region インスタンス呼び出し
	//インスタンス呼び出し
	Input* input = Input::GetInstance();
#pragma endregion

#pragma region 魔法の削除
	//消すフラグの立った魔法の削除
	fireBalls_.remove_if([](unique_ptr<FireBall>& fireBall) {
		return fireBall->GetIsDead();
	});
	magicMissiles_.remove_if([](unique_ptr<MagicMissile>& magicMissile) {
		return magicMissile->GetIsDead();
	});
#pragma endregion

#pragma region 魔法のアップデート
	//各魔法の発射
	if (isUseFireBall_) {
		FireFireBall();
	}
	if (isUseMagicMissile_) {
		FireMagicMissile();
	}

	//各魔法のUpdate
	for (unique_ptr<FireBall>& fireBall : fireBalls_) {
		fireBall->Update();
	}
	for (unique_ptr<MagicMissile>& magicMissile : magicMissiles_) {
		magicMissile->Update();
	}
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

bool SpellManager::GetIsUseSpell()
{
	if (isUseFireBall_ || isUseMagicMissile_) {
		return true;
	}
	return false;
}
