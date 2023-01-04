#pragma once
#include "ISceneManager.h"

#include <memory>

class GameManager
{
private:
	static std::unique_ptr<ISceneManager> scene_;

public:
	static void SetTitleScene();
	static void SetGameScene();

public:
	GameManager();

	void Update(Input* input);
	void Draw();
	void SpriteDraw();
	void Delete();
	
};

