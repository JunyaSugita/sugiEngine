/**
 * @file LoadOut.h
 * @brief 装備する魔法を変えたり装備している魔法を発射させたりを管理する
 */

#pragma once
#include "SpellManager.h"
#include "Sprite.h"

enum Mode {
	SELECT_SPELL,
	SELECT_NUM,
};

class LoadOut final{
private:
	LoadOut() = default;
	~LoadOut() = default;
public:
	//コピーコンストラクタ無効
	LoadOut(const LoadOut& obj) = delete;
	//代入演算子を無効
	LoadOut& operator=(const LoadOut& obj) = delete;

	static LoadOut* GetInstance();

public:
	void Initialize();
	void Update();
	void BackDrawSp();
	void Draw();

	bool GetIsActive() {
		return isActive_;
	}
	void SetIsActive(bool is) {
		isActive_ = is;
	}
	void ToggleIsActive() {
		ResetWindow();
		isActive_ = (isActive_ + 1) % 2;
	}

	bool GetIsDirty() {
		return isDirty_;
	}
	void SetIsDirty(bool is) {
		isDirty_ = is;
	}

	int32_t GetSpell(int32_t num) {
		return setSpell_[num];
	}
	int32_t GetSelectMode() {
		return selectMode_;
	}

	void ResetWindow();

private:
	void SetSpell(int32_t num,int32_t spellName);

private:
	//装備画面かどうか
	bool isActive_;

	int32_t setSpell_[5];//各枠の保有魔法
	int32_t selectSpell_;//変える魔法を保持
	int32_t selectNum_;//変える枠番
	int32_t selectMode_;//変更状況
	bool isDirty_;//変えたかどうか

	static const int SPELL_ALL = 16;
	static const int SPELL_SET = 4;

	int32_t spellTexNum_[SPELL_ALL];
	Sprite preview_[SPELL_ALL];
	Sprite hiLight_;

	Sprite set_[SPELL_SET];
	Sprite back_;
	Sprite SpellExplanation_;
	int32_t spellExplanTex_[SPELL_ALL];

	float preWindowTimer_;

	bool isFirst_ = false;
};
