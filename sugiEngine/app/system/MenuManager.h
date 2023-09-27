#pragma once
#include "Sprite.h"

class MenuManager {
private:
	MenuManager() = default;
	~MenuManager() = default;

public:
	//�R�s�[�R���X�g���N�^����
	MenuManager(const MenuManager& obj) = delete;
	//������Z�q�𖳌�
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

private:
	static const int32_t MAX_MENU = 3;

	bool isActive_;
	bool isReset_;
	bool isStageSelect_;

	bool isResetCheck_;
	bool isStageSelectBackCheck_;

	int32_t menuNum_;

	int32_t selectNum_;
	int32_t checkNum_;

	Sprite menuTex_[MAX_MENU];
	Sprite backSp_;

	int32_t backTex_;
	int32_t resetTex_;
	int32_t stageSelectTex_;

	int32_t timer_;
};