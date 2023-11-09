#pragma once
#include "Sprite.h"

class Tutorial final{
private:
	Tutorial() = default;
	~Tutorial() = default;
public:
	//コピーコンストラクタ無効
	Tutorial(const Tutorial& obj) = delete;
	//代入演算子を無効
	Tutorial& operator=(const Tutorial& obj) = delete;

	static Tutorial* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	bool GetIsTutorial() {
		return isTutorial_;
	}
	void SetIsTutorial(bool is) {
		isTutorial_ = is;
	}

	bool GetIsReturn() {
		return isReturn_;
	}
	void SetIsReturn(bool is) {
		isReturn_ = is;
	}
private:
	uint32_t tex_[5][2];
	uint32_t texStart_;

	int32_t animeNum_;

	const int32_t ANIME_TIME = 60;
	int32_t animeTimer_;

	Sprite sprite_;
	Sprite sprite2_;

	Sprite gotoLoadOut_;

	uint8_t number_;
	bool isNext_;

	bool isTutorial_ = true;
	bool isReturn_ = false;
};
