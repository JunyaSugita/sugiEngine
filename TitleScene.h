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
	void Delete() override;

public:

private:
	//スプライトデータ
	uint32_t titleTexture_;
	Sprite titleSprite_;

	uint32_t tutorialTexture_;
	Sprite tutorialSprite_;

	uint32_t maingameTexture_;
	Sprite maingameSprite_;
};

