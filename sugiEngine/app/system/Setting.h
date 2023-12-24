/**
 * @file Setting.h
 * @brief 設定を管理するマネージャー
 */

#pragma once
#include "GrovalSetting.h"
#include "Sprite.h"
#include "Slider.h"

enum Set {
	BGM,
	SE,
	XSensi,
	YSensi,
	XReverse,
	YReverse,
};

class Setting
{
private:
	Setting() = default;
	~Setting() = default;

public:
	//コピーコンストラクタ無効
	Setting(const Setting& obj) = delete;
	//代入演算子を無効
	Setting& operator=(const Setting& obj) = delete;

	static Setting* GetInstance();

	void Initialize();
	void Update();
	void Draw();

	bool GetIsActive() {
		return isActive_;
	}
	float GetXSensi() {
		return sensiX_;
	}
	float GetYSensi() {
		return sensiY_;
	}
private:
	//設定画面を開いているかどうか
	bool isActive_ = false;
	//カメラ感度調整
	float sensiX_ = 1.0f;
	float sensiY_ = 1.0f;
	//カメラ反転切替
	bool isReverseX_ = false;
	bool isReverseY_ = false;

	//メニューの操作
	const int32_t TIME_MOVE = 15;
	const int32_t MAX_MENU = 6;
	
	int32_t menuNum_;
	int32_t timer_;
	
	Sprite sprite_[4];
	Slider slider_[4];
	float BGMVol_ = 0.8f;
	float SEVol_ = 0.8f;
	float XSensi_ = 1.0f;
	float YSensi_ = 1.0f;
};

