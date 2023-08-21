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
	if (Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_DPAD_UP)) {
		isUse_ = true;
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
	Cancel();
}

void ItemManager::ChangeItem()
{
	if (Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_DPAD_LEFT)) {
		itemType_--;
		if (itemType_ < 0) {
			itemType_ = ITEM_END - 1;
		}
	}
	if (Input::GetInstance()->TriggerButton(XINPUT_GAMEPAD_DPAD_RIGHT)) {
		itemType_++;
		if (itemType_ >= ITEM_END) {
			itemType_ = 0;
		}
	}
}
