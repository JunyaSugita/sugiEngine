#pragma once
#include "GrovalSetting.h"
#include 

enum Item {
	HEAL_LV1,
	HEAL_LV2,
	HEAL_LV3,
	PROTECT_LV1,
	PROTECT_LV2,
	ITEM_END
};

class ItemManager {
private:
	ItemManager() = default;
	~ItemManager() = default;
public:
	//コピーコンストラクタ無効
	ItemManager(const ItemManager& obj) = delete;
	//代入演算子を無効
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

	//インライン関数
	bool GetIsUse() {
		return isUse_;
	}

private:
	const float TIME_USE = 60 * 3.0f;

	bool isUse_;
	float timer_;
	int32_t itemType_;

	BaseObj obj_;
};