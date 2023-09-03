#pragma once
#include "GrovalSetting.h"
#include "BaseObj.h"
#include "Gauge.h"

enum Item {
	HEAL_LV1,
	HEAL_LV2,
	HEAL_LV3,
	PROTECT_LV1,
	PROTECT_LV2,
	END_ITEM
};

class ItemManager {
private:
	ItemManager() = default;
	~ItemManager() = default;
public:
	//�R�s�[�R���X�g���N�^����
	ItemManager(const ItemManager& obj) = delete;
	//������Z�q�𖳌�
	ItemManager& operator=(const ItemManager& obj) = delete;

	static ItemManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();
	void DrawSprite();

	void Use();
	void Cancel();
	void EffectActive();
	void ChangeItem();

	//�C�����C���֐�
	bool GetIsUse() {
		return isUse_;
	}

private:
	const float TIME_USE = 60 * 3.0f;

	bool isUse_;
	float timer_;
	int32_t itemType_;

	BaseObj obj_;
	bool isRot_;
	float objRot_;

	int32_t haveItem_[END_ITEM];

	Gauge gauge_;

	Sprite itemIcon_;
	Sprite itemNum_;
	Sprite padIcon_;
	int numberTex_[4];
};