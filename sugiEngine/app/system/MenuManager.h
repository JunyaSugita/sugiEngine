#pragma once
#include "Sprite.h"

class MenuManager {
private:
	MenuManager() = default;
	~MenuManager() = default;

public:
	//コピーコンストラクタ無効
	MenuManager(const MenuManager& obj) = delete;
	//代入演算子を無効
	MenuManager& operator=(const MenuManager& obj) = delete;

	static MenuManager* GetInstance();

	void SetIsActive(bool is) {
		isActive_ = is;
	}
	bool GetIsActive() {
		return isActive_;
	}

	void SetIsReset(bool is) {
		isActive_ = is;
	}
	bool GetIsReset() {
		return isReset_;
	}

	void Initialize();
	void Update();
	void Draw();

	void SetGameManu();

	void Back();
	void Reset();
	void GoStageSelect();
	void GoSpellSetting();
	void BackStage();
	void Enter();
	void Cancel();

private:
	static const int32_t TEX_NUM = 3;

	bool isActive_;
	bool isReset_;

	bool isCheck_;

	int32_t menuNum_;
	int32_t selectNum_;

	Sprite menuTex_[TEX_NUM];
};