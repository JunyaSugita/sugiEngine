#pragma once
#include "Sprite.h"

class UIManager final{
private:
	UIManager() = default;
	~UIManager() = default;
public:
	//コピーコンストラクタ無効
	UIManager(const UIManager& obj) = delete;
	//代入演算子を無効
	UIManager& operator=(const UIManager& obj) = delete;

	static UIManager* GetInstance();

public:
	void Initialize();
	void GameInitialize();
	void Update();
	void Draw();

	void SetClear();
	void SetGameOver();
	float GetStateAlpha_() {
		return stateAlpha_;
	}
private:
	Sprite stateSp_;
	float stateAlpha_;

	int32_t clearTex_;
	int32_t gameOverTex_;
};