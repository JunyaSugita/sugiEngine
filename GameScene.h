#pragma once
#include "ISceneManager.h"
#include "Model.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "GameManager.h"
#include "Sound.h"
#include "LightGroup.h"

class GameScene : public ISceneManager
{
public:
	void Initialize(int num) override;
	void Update(Input* input) override;
	void BackSpriteDraw() override;
	void Draw() override;
	void SpriteDraw() override;
	void Delete() override;

public:

private:
	Model* model_;
	Object3d* obj_;
	WorldTransform worldTransform_;

	uint32_t catTexture_;
	Sprite catSprite_;

	std::unique_ptr<Sound> sound = std::make_unique <Sound>();

	LightGroup* lightGroup_ = nullptr;
};

