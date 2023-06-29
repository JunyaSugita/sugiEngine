#pragma once
#include "Sprite.h"
#include "GrovalSetting.h"

class Closshair {
private:
	Closshair();
	~Closshair();
public:
	//コピーコンストラクタ無効
	Closshair(const Closshair& obj) = delete;
	//代入演算子を無効
	Closshair& operator=(const Closshair& obj) = delete;

	static Closshair* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

private:
	uint32_t clossTex_;
	Sprite clossSprite_;
};