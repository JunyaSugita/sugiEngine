#pragma once
#include "ISceneManager.h"

#include <memory>

class GameManager
{
private:
	static std::unique_ptr<ISceneManager> scene_;
	static int stage_;
public:
	static void SetTitleScene();
	static void SetStageSelectScene();
	static void SetGameScene();
	static void SetClearScene();

	static void SetStage(int stageNum) {
		stage_ = stageNum;
	};

public:
	GameManager();

	void Update(Input* input);
	void BackSpriteDraw();
	void Draw();
	void SpriteDraw();
	void Delete();
	
};

