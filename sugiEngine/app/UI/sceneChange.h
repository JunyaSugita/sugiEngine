#pragma once
#include "Sprite.h"

class sceneChange {
private:
	sceneChange();
	~sceneChange();
public:
	//�R�s�[�R���X�g���N�^����
	sceneChange(const sceneChange& obj) = delete;
	//������Z�q�𖳌�
	sceneChange& operator=(const sceneChange& obj) = delete;

	static sceneChange* GetInstance();

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