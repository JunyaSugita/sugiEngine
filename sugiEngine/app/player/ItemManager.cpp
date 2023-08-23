#include "ItemManager.h"
#include "Input.h"
#include "Player.h"

ItemManager* ItemManager::GetInstance()
{
	static ItemManager instance;

	return &instance;
}

void ItemManager::Initialize()
{
	isUse_ = false;
	timer_ = 0.0f;
	itemType_ = HEAL_LV1;
	for (int i = 0; i < END_ITEM;i++) {
		haveItem_[i] = 0;
	}
	haveItem_[HEAL_LV1] = 3;
}

void ItemManager::Update()
{
	if (isUse_) {
		if (++timer_ >= TIME_USE) {
			EffectActive();
		}
	}
	else {
		ChangeItem();
	}
}

void ItemManager::Draw()
{

}

void ItemManager::Use()
{
	Player* player = Player::GetInstance();
	if (Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_DPAD_UP) && haveItem_[itemType_] > 0) {
		switch (itemType_)
		{
		case HEAL_LV1:
		case HEAL_LV2:
		case HEAL_LV3:
			if (player->GetMaxLife() > player->GetLife()) {
				isUse_ = true;
			}
			break;

		case PROTECT_LV1:
		case PROTECT_LV2:
			isUse_ = true;
			break;
		}
	}
}

void ItemManager::Cancel()
{
	isUse_ = false;
	timer_ = 0;
};

void ItemManager::EffectActive()
{
	switch (itemType_)
	{
	case HEAL_LV1:
		Player::GetInstance()->AddHealNum(1000);
		break;
	case PROTECT_LV1:

		break;
	default:
		break;
	}
	haveItem_[itemType_]--;
	Cancel();
}

void ItemManager::ChangeItem()
{
	if (Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_DPAD_LEFT)) {
		itemType_--;
		if (itemType_ < 0) {
			itemType_ = END_ITEM - 1;
		}
	}
	if (Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_DPAD_RIGHT)) {
		itemType_++;
		if (itemType_ >= END_ITEM) {
			itemType_ = 0;
		}
	}
}
