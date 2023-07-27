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


private:
	uint32_t tex_;
	uint32_t tex2_;
	uint32_t tex3_;
	uint32_t tex4_;
	uint32_t tex5_;

	Sprite sprite_;

	int32_t time_;
	uint8_t number_;
	bool isNext_;
};