#include "SpellManager.h"
#include "Input.h"
#include "Player.h"
#include "PlayerWeapon.h"
#include "LoadOut.h"
#include "ImGuiManager.h"
#include "Status.h"
#include "FireBall.h"
#include "MagicMissile.h"
#include "IceBolt.h"
#include "EnchantFire.h"
#include "Flame.h"
#include "Wind.h"

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
	for (int i = 0; i < MAGIC_END; i++) {
		isUseSpell_[i] = false;
	}

	Status* status = Status::GetInstance();

	for (int i = 0; i < MAGIC_END; i++) {
		timeChargeSpell_[i] = status->GetSpellData(i).timeCharge;
		timeFireSpell_[i] = status->GetSpellData(i).timeShot;
	}
}

void SpellManager::Update()
{

#pragma region 状態のリセット
	isModeEnchantFire_ = false;

#pragma endregion

#pragma region 魔法のアップデート
	//各魔法の発射
	if (isUseSpell_[FIRE_BALL]) {
		FireFireBall();
	}
	if (isUseSpell_[MAGIC_MISSILE]) {
		FireMagicMissile();
	}
	if (isUseSpell_[ICE_BOLT]) {
		FireIceBolt();
	}
	if (isUseSpell_[CHAIN_LIGHTNING]) {
		FireChainLightning();
	}
	if (isUseSpell_[ENCHANT_FIRE]) {
		FireEnchantFire();
	}
	if (isUseSpell_[FLAME]) {
		FireFlame();
	}
	if (isUseSpell_[WIND]) {
		FireWind();
	}

	//各魔法のUpdate
	for (unique_ptr<BaseSpell>& spell : spells_) {
		spell->Update();
	}
	for (unique_ptr<ChainLightning>& chainLightning : chainLightnings_) {
		chainLightning->Update();
	}

#pragma endregion

#pragma region 魔法の削除
	//消すフラグの立った魔法の削除
	for (int i = 0; i < spells_.size();) {
		if (spells_[i]->GetIsDead()) {
			spells_.erase(spells_.begin() + i);
		}
		else {
			i++;
		}
	}
	for (int i = 0; i < chainLightnings_.size();) {
		if (chainLightnings_[i]->GetIsDead()) {
			chainLightnings_.erase(chainLightnings_.begin() + i);
		}
		else {
			i++;
		}
	}

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

void SpellManager::ChargeSpell(int32_t spell)
{
	maxCharge_ = timeChargeSpell_[spell];

	if (chargeTime_ < maxCharge_) {
		chargeTime_++;
	}

	if ((!Input::GetInstance()->PushKey(DIK_E) && !Input::GetInstance()->PushButton(XINPUT_GAMEPAD_LEFT_SHOULDER) && !Input::GetInstance()->GetLTrigger()) || (LoadOut::GetInstance()->GetIsActive() && chargeTime_ >= maxCharge_)) {
		if (int(chargeTime_ / maxCharge_)) {
			isUseSpell_[spell] = true;
			useTime_ = timeFireSpell_[spell];
		}
		if (!(SpellManager::GetInstance()->ChargePercent() > COYOTE_SPELL && SpellManager::GetInstance()->ChargePercent() < 1) && (isUseSpell_[spell] || !LoadOut::GetInstance()->GetIsActive())) {
			chargeTime_ = 0;
		}
	}
}

void SpellManager::FireFireBall()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = timeFireSpell_[FIRE_BALL];

	if (int(useTime_) == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<FireBall>();
		newSpell->Initialize(Player::GetInstance()->GetWeapon()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseSpell_[FIRE_BALL] = false;
		Player::GetInstance()->SetIsSpell(false);
	}

}

void SpellManager::FireMagicMissile()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = timeFireSpell_[MAGIC_MISSILE];

	if (int(useTime_) % COOLTIME_MAGIC_MISSILE == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<MagicMissile>();
		newSpell->Initialize(Player::GetInstance()->GetWeapon()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseSpell_[MAGIC_MISSILE] = false;
		Player::GetInstance()->SetIsSpell(false);
	}

}

void SpellManager::FireIceBolt()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = timeFireSpell_[ICE_BOLT];

	if (int(useTime_) % COOLTIME_ICE_BOLT == 1 && useTime_ < 40) {
		unique_ptr<BaseSpell> newSpell = make_unique<IceBolt>();
		newSpell->Initialize(Player::GetInstance()->GetWeapon()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseSpell_[ICE_BOLT] = false;
		Player::GetInstance()->SetIsSpell(false);
	}
}

void SpellManager::FireChainLightning()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = timeFireSpell_[CHAIN_LIGHTNING];

	if (int(useTime_) == 1) {
		unique_ptr<ChainLightning> newSpell = make_unique<ChainLightning>();
		newSpell->Initialize(Player::GetInstance()->GetWeapon()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		chainLightnings_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseSpell_[CHAIN_LIGHTNING] = false;
		Player::GetInstance()->SetIsSpell(false);
	}

}

void SpellManager::FireEnchantFire()
{
	maxCharge_ = timeFireSpell_[ENCHANT_FIRE];

	if (int(useTime_) == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<EnchantFire>();
		newSpell->Initialize(Player::GetInstance()->GetWeapon()->GetOrbPos());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseSpell_[ENCHANT_FIRE] = false;
		Player::GetInstance()->SetIsSpell(false);
	}

}

void SpellManager::FireFlame()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = timeFireSpell_[FLAME];

	if (int(useTime_) % COOLTIME_FLAME == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<Flame>();
		newSpell->Initialize(Player::GetInstance()->GetWeapon()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseSpell_[FLAME] = false;
		Player::GetInstance()->SetIsSpell(false);
	}
}

void SpellManager::FireWind()
{
	Camera* camera = Camera::GetInstance();

	maxCharge_ = timeFireSpell_[WIND];

	if (int(useTime_) == 1) {
		unique_ptr<BaseSpell> newSpell = make_unique<Wind>();
		newSpell->Initialize(Player::GetInstance()->GetWeapon()->GetOrbPos(), camera->GetTarget() - camera->GetEye());
		newSpell->Fire();

		spells_.push_back(move(newSpell));
	}
	if (--useTime_ <= 0) {
		useTime_ = 0;
		isUseSpell_[WIND] = false;
		Player::GetInstance()->SetIsSpell(false);
	}

}

float SpellManager::ChargePercent()
{
	if (maxCharge_ == 0.0f) {
		return 0.0f;
	}
	float temp = chargeTime_ / maxCharge_;
	if (temp >= 1.0f) {
		return 1.0f;
	}
	return temp;
};

bool SpellManager::GetIsUseSpell()
{
	for (int i = 0; i < MAGIC_END; i++) {
		if (isUseSpell_[i]) {
			return true;
		}
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
