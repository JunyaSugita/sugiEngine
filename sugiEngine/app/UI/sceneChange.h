#pragma once
#include "Sprite.h"

class SceneChange final{
private:
	SceneChange() = default;
	~SceneChange() = default;
public:
	//�R�s�[�R���X�g���N�^����
	SceneChange(const SceneChange& obj) = delete;
	//������Z�q�𖳌�
	SceneChange& operator=(const SceneChange& obj) = delete;

	static SceneChange* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	void Start() {
		isSceneChange_ = true;
	}

	bool GetIsSceneChange() {
		return isSceneChange_;
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