#pragma once
#include "Sprite.h"

class SceneChange {
private:
	SceneChange();
	~SceneChange();
public:
	//コピーコンストラクタ無効
	SceneChange(const SceneChange& obj) = delete;
	//代入演算子を無効
	SceneChange& operator=(const SceneChange& obj) = delete;

	static SceneChange* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	void Start() {
		isSceneChange_ = true;
	}

	float GetTimer() {
		return easeTimer_;
	}

public:
	const float SPEED_TIMER = 0.025f;

private:
	Sprite sceneSp_;
	uint32_t sceneTex_;

	Vector2 pos_ = { 0,-WIN_HEIGHT };

	bool isSceneChange_ = false;

	float easeTimer_ = 0;
};