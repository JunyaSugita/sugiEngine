#pragma once
#include "SugiFramework.h"

class MyGame : public SugiFramework
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	void GameDraw();
};

