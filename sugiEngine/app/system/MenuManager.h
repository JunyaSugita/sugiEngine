/**
 * @file MenuManager.h
 * @brief メニュー画面を管理するマネージャー
 */

#pragma once
#include "Sprite.h"

enum MENU{
	BACK,
	RESET,
	STAGE_SELECT,
	SETTING,

	MENU_END
};

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

	bool GetIsReset() {
		return isReset_;
	}
	bool GetIsStageSelect() {
		return isStageSelect_;
	}

	bool GetIsMenu();

	void Initialize();
	void GameInitialize();
	void Update();
	void Draw();

	void SetGameMenu();
	void SetResetCheck();
	void SetStageSelectBackCheck();

	void Back();
	void Reset();
	void GoStageSelect();
	void GoSpellSetting();
	void BackStage();
	void Enter();
	void Cancel();

	void GoToSetting();

private:
	static const int32_t MAX_MENU = 4;
	//カーソル移動のディレイ
	const int32_t TIME_DELAY = 10;
	const int32_t STICK_DELAY = 10000;
	//カーソルサイズ
	const Vector2 SIZE_MENU = { 500, 150 };
	const Vector2 SIZE_BIG_MENU = { 550, 165 };

	bool isActive_;
	bool isReset_;
	bool isStageSelect_;

	bool isResetCheck_;
	bool isStageSelectBackCheck_;

	int32_t selectNum_;
	int32_t checkNum_;

	Sprite menuTex_[MAX_MENU];
	Sprite backSp_;

	int32_t backTex_;
	int32_t resetTex_;
	int32_t stageSelectTex_;
	int32_t settingTex_;

	int32_t timer_;
};
