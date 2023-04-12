#pragma once
#include "Input.h"
#include "ImGuiManager.h"

class ISceneManager
{
public:
	virtual void Initialize(int num) = 0;
	virtual void Update(Input* input, ImGuiManager* imGui) = 0;
	virtual void BackSpriteDraw() = 0;
	virtual void Draw() = 0;
	virtual void SpriteDraw() = 0;
	virtual void Delete() = 0;

};

