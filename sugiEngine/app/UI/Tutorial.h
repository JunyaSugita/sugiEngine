/**
 * @file Tutorial.h
 * @brief チュートリアルの描画(UI)
 */

#pragma once
#include "Sprite.h"

enum TUTORIAL {
	TUTORIAL0,
	TUTORIAL1,
	TUTORIAL2,
	TUTORIAL3,
	TUTORIAL4,

	TUTORIAL_END
};

enum ANIME {
	ANIME0,
	ANIME1,

	ANIME_END
};

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
	const float TUTORIAL_0TO1 = 60;
	const float TUTORIAL_1TO2 = 20;
	const float TUTORIAL_2TO3 = 100;
	const float TUTORIAL_3TO4 = 60;

	uint32_t tex_[TUTORIAL_END][ANIME_END];
	uint32_t texStart_;

	int32_t animeNum_;

	const int32_t ANIME_TIME = 60;
	int32_t animeTimer_;

	Sprite sprite_;
	Sprite sprite2_;
	Sprite sprite3_;
	uint32_t tex2_;

	Sprite gotoLoadOut_;

	uint8_t number_;
	bool isNext_;

	bool isTutorial_ = true;
	bool isReturn_ = false;
	bool isShowSprite3_;
};
