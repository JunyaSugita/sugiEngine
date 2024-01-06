/**
 * @file UIManager.h
 * @brief UIの描画を管理するマネージャー
 */

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

	//クリアを呼び出し
	void SetClear();
	//ゲームオーバーを呼び出し
	void SetGameOver();
	//透明度
	float GetStateAlpha_() {
		return stateAlpha_;
	}
private:
	//ゲームオーバー、クリアの出るスピード
	const float SPEED_ALPHA = 0.025f;

	Sprite stateSp_;
	float stateAlpha_;

	Sprite gotoLoadOut_;

	int32_t clearTex_;
	int32_t gameOverTex_;
};
