#pragma once
#include "GrovalSetting.h"
#include "BaseObj.h"

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

	int32_t haveItem_[END_ITEM];
};