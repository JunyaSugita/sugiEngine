#pragma once
#include "ISceneManager.h"
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"

class TitleScene : public ISceneManager
{
public:
	void Initialize() override;
	void Update(Input* input) override;
	void Draw() override;
	void SpriteDraw() override;

public:

private:

};

