#pragma once
#include "GrovalSetting.h"
#include "Sprite.h"

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


private:
	//設定画面を開いているかどうか
	bool isSetting_ = false;
	//カメラ感度調整
	float sensiX_ = 1.0f;
	float sensiY_ = 1.0f;
	//カメラ反転切替
	bool isReverseX_ = false;
	bool isReverseY_ = false;

	//メニューの操作
	const int32_t TIME_MOVE = 50;
	const int32_t MAX_MENU = 6;
	
	int32_t menuNum_;
	int32_t timer_;
	

	Sprite slider_[4];
};

