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

	obj_.Initialize("posion");
	obj_.obj->SetColor({0.5f,0.2f,0.2f,1});

	itemIcon_.Initialize(Sprite::LoadTexture("healPosionIcon.png"));
	itemIcon_.SetPos({900,660});

	numberTex_[0] = Sprite::LoadTexture("number1.png");
	numberTex_[1] = Sprite::LoadTexture("number1.png");
	numberTex_[2] = Sprite::LoadTexture("number2.png");
	numberTex_[3] = Sprite::LoadTexture("number3.png");

	itemNum_.Initialize(numberTex_[3]);
	itemNum_.SetSize(25,25);
	itemNum_.SetPos({ 925,680 });

	padIcon_.Initialize(Sprite::LoadTexture("PAD_UP_ICON.png"));
	padIcon_.SetPos({900,610});

	gauge_.Set({WIN_WIDTH / 2,525},{300,50},{0.2f,0.5f,0.2f});
}

void ItemManager::Update()
{
	if (isUse_) {
		if (++timer_ >= TIME_USE) {
			EffectActive();
		}
		if (objRot_ < 80 && isRot_ == false) {
			objRot_++;
		}
		else{
			isRot_ = true;
			if (objRot_ > 0) {
				objRot_--;
			}
		}
	}
	else {
		ChangeItem();
		objRot_ = 0;
		isRot_ = false;
	}


	Player* player = Player::GetInstance();

	obj_.pos = player->GetPos();
	obj_.pos.x += float(sin(Radian(player->GetCameraAngle().x)) * 4);
	obj_.pos.y = 1.0f + sinf(timer_ / 60) * 4;
	obj_.pos.z += float(cos(Radian(player->GetCameraAngle().x)) * 4);
	obj_.rot = { max(-objRot_ * 2,-110) ,player->GetCameraAngle().x,0};

	obj_.Update();
	gauge_.Update(TIME_USE,timer_);
}

void ItemManager::Draw()
{
	if (isUse_) {
		obj_.Draw();
	}
}

void ItemManager::DrawSprite()
{
	if (isUse_) {
		gauge_.Draw();
	}
	if (haveItem_[itemType_]) {
		padIcon_.Draw();
		itemIcon_.Draw();
		itemNum_.Draw();
	}
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
	itemNum_.SetTexture(numberTex_[haveItem_[itemType_]]);
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
