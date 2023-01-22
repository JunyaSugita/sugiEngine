#pragma once
#include "ISceneManager.h"
#include "Object3d.h"
#include "Sprite.h"
#include "GameManager.h"

class ClearScene : public ISceneManager
{
public:
	void Initialize(int num) override;
	void Update(Input* input) override;
	void BackSpriteDraw() override;
	void Draw() override;
	void SpriteDraw() override;
	void Delete() override;

private:

};

