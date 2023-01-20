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
	int clearNum_;

	//スプライトデータ
	uint32_t clearTexture_;
	Sprite clearSprite_;

	uint32_t tutorialTexture_;
	Sprite tutorialSprite_;

	uint32_t easyTexture_;
	Sprite easySprite_;

	uint32_t normalTexture_;
	Sprite normalSprite_;

	uint32_t hardTexture_;
	Sprite hardSprite_;

	uint32_t pressTexture_;
	Sprite pressSprite_;
};

