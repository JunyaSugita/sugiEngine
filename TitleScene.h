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
	void Initialize(int num) override;
	void Update(Input* input) override;
	void BackSpriteDraw() override;
	void Draw() override;
	void SpriteDraw() override;
	void Delete() override;

private:
	//スプライトデータ
	uint32_t titleTexture_;
	Sprite titleSprite_;
	uint32_t pressTexture_;
	Sprite pressSprite_;
};

