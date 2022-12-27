#pragma once
#include "Input.h"

class ISceneManager
{
public:
	virtual void Initialize() = 0;
	virtual void Update(Input* input) = 0;
	virtual void Draw() = 0;
	virtual void SpriteDraw() = 0;

};

